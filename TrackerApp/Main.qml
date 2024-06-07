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

    function updateImageSource() {
        videoFrame.source = "image://imageProvider/frame?cache=" + Date.now();
    }

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

                //myModel.onConnect();
                myModel.startVideo();
                // TODO: below video logic needs to be changed to access video receiver
                //videoPlayer.visible = true  // Make the video player visible
                //videoPlayer.play();  // Start playing the video
                //videoFrame.visible = true; // Make the video frame visible
                myModel.openUART();
                connectButton.visible = false;
                startTrackerButton.visible = true;
            }
            anchors.horizontalCenter: parent.horizontalCenter
            Material.background: Material.primaryColor// Set the background color to a custom color
            Material.foreground: "white" // Set the text color to white
        }

        // Video {
        //     id: videoPlayer
        //     width: parent.width  // Make the video player the same width as the parent
        //     height: 300  // Fixed height for the video player
        //     source: "C:/Users/kirra/OneDrive/Desktop/TrackerQTApp/TrackerApp/TrackerApp/walking.mp4"
        //     visible: false
        // }

        Button {
            id: saveButton
            text: "Save Frame"
            width: 150  // Width of the button in pixels
            height: 50   // Height of the button in pixels
            highlighted: true  // Highlight the button with accent color
            Material.elevation: 2  // Apply elevation for shadow effect
            onClicked: {
                myModel.saveFrame("../../captured_frame.png");
            }
            anchors.horizontalCenter: parent.horizontalCenter
            Material.background: Material.primaryColor // Set the background color to a custom color
            Material.foreground: "white" // Set the text color to white
        }

        Image {
            id: videoFrame
            width: parent.width
            height: 300
            source: "image://imageProvider/frame"  // Correct provider path
            visible: true
            fillMode: Image.PreserveAspectFit
            onSourceChanged: {
                console.log("Frame source changed to: " + videoFrame.source);
            }
        }

        // This shows image:
        // Image {
        //     id: imageFrame
        //     width: parent.width
        //     height: 300
        //     source: "images/captured_frame.png"  // Use the correct resource path
        //     visible: true
        //     fillMode: Image.PreserveAspectFit
        //     onSourceChanged: console.log("Image source changed to: " + imageFrame.source)
        // }

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
                mainText.text = "Draw bounding box around object you would like to track."
                // TODO: write logic to allow user to draw bbox and get p1 and p2 coordinates from it
                // pass message over uart with x1,y1 x2,y2 data
                myModel.writeUART("track-start 448 261 528 381\n");
                mainText.text = "Tracking in progress";
                stopTrackerButton.visible = true;
            }
            anchors.horizontalCenter: parent.horizontalCenter
            Material.background: Material.primaryColor// Set the background color to a custom color
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
            Material.background: Material.primaryColor// Set the background color to a custom color
            Material.foreground: "white" // Set the text color to white
            visible: false
        }

    }

    onWidthChanged: {
        mainText.font.pixelSize = width / 20  // Make the font size responsive
    }
}

