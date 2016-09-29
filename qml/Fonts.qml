import QtQuick 2.0

Item {
    property alias adobeClean: adobeClean
    property alias segoeUi: segoeUi

    FontLoader { id: adobeClean; source: "qrc:/assets/fonts/AdobeClean-Light.otf" }
    FontLoader { id: segoeUi; source: "qrc:/assets/fonts/segoeuil.ttf" }
}
