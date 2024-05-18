import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15  // Import the Material style module
import QtMultimedia 6.7

ApplicationWindow {
    visible: true
    width: 640
    height: 700
    title: "Mobile Tracking System"

    // Setting a Material style theme
    Material.theme: Material.Dark  // Choose between Light, Dark, or System themes
    Material.primary: "0096FF"

    Column {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 40

        Text {
            id: mainText
            text: "Welcome!"
            font.pixelSize: 46
            anchors.horizontalCenter: parent.horizontalCenter
            color: Material.primaryColor  // Use accent color for the text
        }

        Button {
            id: connectButton
            text: "Connect"
            width: 150  // Width of the button in pixels
            height: 50   // Height of the button in pixels
            highlighted: true  // Highlight the button with accent color
            Material.elevation: 2  // Apply elevation for shadow effect
            onClicked: {
                //myModel.setData(myModel.data + 1);

                myModel.onConnect();

                videoPlayer.visible = true  // Make the video player visible
                videoPlayer.play();  // Start playing the video
                myModel.openUART();
                myModel.writeUART();
            }
            anchors.horizontalCenter: parent.horizontalCenter
            Material.background: Material.primaryColor// Set the background color to a custom color
            Material.foreground: "white" // Set the text color to white
        }

        Video {
            id: videoPlayer
            width: parent.width  // Make the video player the same width as the parent
            height: 300  // Fixed height for the video player
            source: "C:/Users/charl/OneDrive/Desktop/TrackerApp/TrackerApp/walking.mp4"
            visible: false
        }

    }

    onWidthChanged: {
        mainText.font.pixelSize = width / 20  // Make the font size responsive
    }
}

