#include "neuralnetwork.h"

using namespace NN;

NNTopologyData::NNTopologyData(QVector<long double> *rawWeightsSigmas, const InternalTopology *topology, long double king)
    :Nw{0}, king{king}
{
    unsigned int rawIndex = 0;
    for(int i = 0; i < topology->size(); i++) //Layer
    {
        LayerData currentLayer;
        for(unsigned int j = 0; j < topology->at(i); j++) //Neuron
        {
            NeuronData neuron;
            unsigned int kSize = (!i ? 32 : topology->at(i - 1)) + 1;
            for(unsigned int k = 0; k < kSize; k++)
            {
                neuron.append(Weight(rawWeightsSigmas->at(rawIndex), rawWeightsSigmas->at(rawIndex + 1)));
                rawIndex += 2;
                Nw += 1;
            }
            currentLayer.append(neuron);
        }
        weights.append(currentLayer);
    }

    NeuronData outputNeuron;
    for(unsigned int i = 0; i < topology->back() + 1; i++)
    {
        outputNeuron.append(Weight(rawWeightsSigmas->at(rawIndex), rawWeightsSigmas->at(rawIndex + 1)));
        rawIndex += 2;
        Nw += 1;
    }
    weights.append(LayerData() << outputNeuron);
}

//---------------------------------------------------------------------------------------------------------

NeuralNetwork::NeuralNetwork(NNTopologyDataPointer data, QObject *parent)
    : QObject(parent), data{data}
{
    for(int i = 0; i < data->weights.length() - 1; i++)
    {
        this->hiddenLayers.append(Layer());
        Layer &lastLayer = this->hiddenLayers.back();

        const auto &layerData = data->weights.at(i);
        for(const auto &neuronData : layerData)
        {
            lastLayer.append(Neuron(&neuronData));
        }
    }

    //Add null weights to outputNeuron to sum the boardInputs
    this->outputNeuronWeights = data->weights.last().last();
    for(int i = 0; i < 32; i++)
    {
        this->outputNeuronWeights.append(Weight(1.0, 1.0));
    }
    this->outputNeuron = Neuron(&this->outputNeuronWeights);
}

NeuralNetwork::~NeuralNetwork()
{

}

long double NeuralNetwork::processBoard(Game::BoardData &board)
{
    InputVector boardInput = createInputVector(board);
    InputVector layerInput;
    InputVector currentOutputs;

    for(int i = 0; i < this->hiddenLayers.size(); i++)
    {
        layerInput = !i ? boardInput : currentOutputs;
        layerInput.append(1.0L); //Add bias input
        currentOutputs.clear();

        Layer &layer = this->hiddenLayers[i];
        for(auto &neuron : layer)
        {
            currentOutputs.append(neuron.process(&layerInput));
        }
    }

    layerInput = currentOutputs;
    layerInput.append(1.0L); //Add bias input
    layerInput += boardInput;
    return this->outputNeuron.process(&layerInput);
}

NeuralNetworkPointer NeuralNetwork::createChild() const
{
    NNTopologyDataPointer NNData = NNTopologyDataPointer(new NNTopologyData());
    NNData->Nw = this->data->Nw;

    long double tau = 1.0L / std::sqrtl(2.0L * std::sqrtl(static_cast<long double>(data->Nw)));

    std::random_device rd;
    std::mt19937_64 randomEngine(rd());
    std::normal_distribution<long double> gaussianDistribution(0.0, 1.0);

    for(const LayerData &layerData : this->data->weights)
    {
        NNData->weights.append(LayerData());
        LayerData &currentLayer = NNData->weights.back();
        for(const NeuronData &neuronData : layerData)
        {
            currentLayer.append(NeuronData());
            NeuronData &currentNeuron = currentLayer.back();
            for(const Weight &weightData : neuronData)
            {
                long double newSigma = weightData.sigma * std::expl(tau * gaussianDistribution(randomEngine));

                currentNeuron.append(Weight(weightData.weight +
                                            newSigma * gaussianDistribution(randomEngine), newSigma));
            }
        }
    }

    NNData->king = this->data->king * std::expl((1 / std::sqrtl(2)) * gaussianDistribution(randomEngine));

    return NeuralNetworkPointer(new NeuralNetwork(NNData));
}

NeuralNetworkPointer NeuralNetwork::createGenerationZeroNN(InternalTopology *topology)
{
    NNTopologyDataPointer NNData = NNTopologyDataPointer(new NNTopologyData());
    NNData->king = 2.0; //King default value
    NNData->Nw = 0;

    std::random_device rd;
    std::mt19937_64 randomEngine(rd());
    long double upperBound = nextafterl(0.2, 0.3); //Workarround due to std lib function samples from [a,b), and [a,b] needed
    std::uniform_real_distribution<long double> uniformDistribution(-0.2, upperBound);

    for(int i = 0; i < topology->size(); i++) //Layer
    {
        LayerData currentLayer;
        for(unsigned int j = 0; j < topology->at(i); j++) //Neuron
        {
            NeuronData neuron;
            unsigned int kSize = (!i ? 32 : topology->at(i - 1)) + 1;
            for(unsigned int k = 0; k < kSize; k++)
            {
                neuron.append(Weight(uniformDistribution(randomEngine), 0.05L));
                NNData->Nw += 1;
            }
            currentLayer.append(neuron);
        }
        NNData->weights.append(currentLayer);
    }

    NeuronData outputNeuron;
    for(unsigned int i = 0; i < topology->back() + 1; i++)
    {
        outputNeuron.append(Weight(uniformDistribution(randomEngine), 0.05L));
        NNData->Nw += 1;
    }
    NNData->weights.append(LayerData() << outputNeuron);

    return NeuralNetworkPointer(new NeuralNetwork(NNData));
}

InputVector NeuralNetwork::createInputVector(const Game::BoardData &boardVector) const
{
    InputVector ret;
    for(const auto &element : boardVector)
    {
        switch(element)
        {
        case Game::Piece::OpKing:
            ret.append(- this->data->king);
            break;
        case Game::Piece::OpPlayer:
            ret.append(-1.0);
            break;
        case Game::Piece::Empty:
            ret.append(0);
            break;
        case Game::Piece::Player:
            ret.append(1.0);
            break;
        case Game::Piece::King:
            ret.append(this->data->king);
            break;
        }
    }
    return ret;
}
