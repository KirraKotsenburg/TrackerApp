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
                myModel.onConnect();
                videoPlayer.visible = true  // Make the video player visible
                videoPlayer.play();  // Start playing the video
                myModel.openUART();
                connectButton.visible = false;
                startTrackerButton.visible = true;
            }
            anchors.horizontalCenter: parent.horizontalCenter
            Material.background: Material.primaryColor // Set the background color to a custom color
            Material.foreground: "white" // Set the text color to white
        }

        Video {
            id: videoPlayer
            width: 800  // Make the video player the same width as the parent
            height: 600  // Fixed height for the video player
            source: "C:/Users/charl/OneDrive/Desktop/TrackerApp/TrackerApp/walking.mp4"
            visible: false

            MouseArea {
                id: mouseArea
                anchors.fill: parent
                enabled: false

                onPressed: {
                    rect.x = mouse.x;
                    rect.y = mouse.y;
                    rect.width = 0;
                    rect.height = 0;
                }

                onPositionChanged: {
                    rect.width = mouse.x - rect.x;
                    rect.height = mouse.y - rect.y;
                }

                onReleased: {
                    var p1 = Qt.point(Math.floor(rect.x), Math.floor(rect.y));
                    var p2 = Qt.point(Math.floor(rect.x + rect.width), Math.floor(rect.y + rect.height));
                    console.log("Coordinates: ", p1, p2);
                    // You can use p1 and p2 to send coordinates over UART
                    myModel.writeUART("track-start " + p1.x + " " + p1.y + " " + p2.x + " " + p2.y + "\n");
                    mainText.text = "Tracking in progress";
                    stopTrackerButton.visible = true;
                    startTrackerButton.visible = false;
                    videoPlayer.play();  // Resume video playback
                    mouseArea.enabled = false;
                }

                Rectangle {
                    id: rect
                    color: "transparent"
                    border.color: "red"
                    border.width: 2
                    visible: mouseArea.enabled
                }
            }
        }

        Button {
            id: startTrackerButton
            text: "Start Tracking"
            width: 150  // Width of the button in pixels
            height: 50   // Height of the button in pixels
            highlighted: true  // Highlight the button with accent color
            Material.elevation: 2  // Apply elevation for shadow effect
            onClicked: {
                startTrackerButton.visible = false;
                mainText.font.pixelSize = 24;
                mainText.text = "Draw bounding box around object you would like to track.";
                videoPlayer.pause();  // Pause the video
                mouseArea.enabled = true;  // Enable mouse area for drawing bounding box
            }
            anchors.horizontalCenter: parent.horizontalCenter
            Material.background: Material.primaryColor // Set the background color to a custom color
            Material.foreground: "white" // Set the text color to white
            visible: false
        }

        Button {
            id: stopTrackerButton
            text: "Stop Tracking"
            width: 150  // Width of the button in pixels
            height: 50   // Height of the button in pixels
            highlighted: true  // Highlight the button with accent color
            Material.elevation: 2  // Apply elevation for shadow effect
            onClicked: {
                stopTrackerButton.visible = false;
                connectButton.visible = true;
                mainText.text = "Welcome!";
                mainText.font.pixelSize = 24;
                myModel.writeUART("track-end\n")
            }
            anchors.horizontalCenter: parent.horizontalCenter
            Material.background: Material.primaryColor // Set the background color to a custom color
            Material.foreground: "white" // Set the text color to white
            visible: false
        }
    }

    onWidthChanged: {
        mainText.font.pixelSize = width / 20  // Make the font size responsive
    }
}
