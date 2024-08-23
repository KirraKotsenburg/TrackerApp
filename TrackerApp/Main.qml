import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15  // Import the Material style module
import QtMultimedia 6.7


ApplicationWindow {
    visible: true
    width: 740
    height: 800
    title: "Mobile Tracking System"

    // Setting a Material style theme
    Material.theme: Material.Dark  // Choose between Light, Dark, or System themes
    Material.primary: Material.Indigo

    function updateImageSource() {
        videoFrame.source = "image://imageProvider/frame?cache=" + Date.now();
    }

    Column {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 40

        Text {
            id: mainText
            text: "Mobile Tracking System"
            font.pixelSize: 46
            anchors.horizontalCenter: parent.horizontalCenter
            color: Material.primaryColor  // Use accent color for the text
        }

        Button {
            id: connectButton
            text: "Connect Video"
            width: 150  // Width of the button in pixels
            height: 50   // Height of the button in pixels
            highlighted: true  // Highlight the button with accent color
            Material.elevation: 2  // Apply elevation for shadow effect
            onClicked: {
                myModel.onConnect(); // For Video Visibility after button click
                myModel.openUART(); // For read and write to UART
                connectButton.visible = false;
                startTrackerButton.visible = true;
            }
            anchors.horizontalCenter: parent.horizontalCenter
            Material.background: Material.primaryColor// Set the background color to a custom color
            Material.foreground: "white" // Set the text color to white
        }

        Image {
            id: videoFrame
            width: 720 //parent.width
            height: 480
            source: "image://imageProvider/frame"  // Correct provider path
            visible: true
            fillMode: Image.PreserveAspectFit

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
                    mainText.text = "Tracking in progress";
                    var p1 = Qt.point(Math.floor(rect.x), Math.floor(rect.y));
                    var p2 = Qt.point(Math.floor(rect.x + rect.width), Math.floor(rect.y + rect.height));
                    console.log("Coordinates: ", p1, p2);

                // These two lines will send the payload over UART
                    var payload = "R track-start " + p1.x + " " + p1.y + " " + p2.x + " " + p2.y + "\n";
                    myModel.payloadPrepare(payload, 101); // 101 is the ACII value of 'e'

                    mainText.color = Material.primaryColor
                    mainText.text = "Tracking in progress";
                    stopTrackerButton.visible = true;
                    startTrackerButton.visible = false;
                    mouseArea.enabled = false;
                    stopTrackerButton.visible = true;
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
                mainText.color = Material.primaryColor
                mainText.text = "Draw bounding box around object you would like to track."
                // TODO: write logic to allow user to draw bbox and get p1 and p2 coordinates from it
                // pass message over uart with x1,y1 x2,y2 data
                mouseArea.enabled = true;  // Enable mouse area for drawing bounding box       
            }
            anchors.horizontalCenter: parent.horizontalCenter
            Material.background: "green" // Set the background color to a custom color
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
                mainText.color = Material.primaryColor
                mainText.text = "Mobile Tracking System";
                mainText.font.pixelSize = 24;

            // These two lines will send the payload over UART
                var payload = "R track-end\n";
                myModel.payloadPrepare(payload, 102);// 102 is the ACII value of 'f'

                startTrackerButton.visible = true;
                rect.width = 0;  // Reset width
                rect.height = 0;  // Reset height
            }
            anchors.horizontalCenter: parent.horizontalCenter
            Material.background: "red" // Set the background color to a custom color
            Material.foreground: "white" // Set the text color to white
            visible: false
        }

        // For failed tracking from raspi
        Connections {
            target: myModel
            onTrackFail: {
                console.log("Tracking failed");
                mainText.color = "red"
                mainText.text = "Tracking failed. Please try again.";
                startTrackerButton.visible = true;
                stopTrackerButton.visible = false;
                mouseArea.enabled = false;
                rect.width = 0;  // Reset width
                rect.height = 0;  // Reset height
            }
        }

    }

    onWidthChanged: {
        mainText.font.pixelSize = width / 20  // Make the font size responsive
    }
}

