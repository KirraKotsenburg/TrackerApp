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

        // Positioning the Row at the bottom
        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            spacing: 10  // Space between ComboBoxes

            ComboBox{
                id: comboCameraBox
                textRole: "name"
                displayText: "Camera Selection"
                model: ListModel {
                    id: comboCameraBoxModel
                    // Initially empty; will be populated dynamically
                }

                onCurrentIndexChanged: {
                    var selectedIndex = comboCameraBoxModel.get(comboCameraBox.currentIndex).index
                    console.log("Selected Camera index:", selectedIndex);
                    myModel.accessCamera(selectedIndex);
                    myModel.onConnect();
                    startTrackerButton.visible = true;
                }
                width: 200
                // anchors.bottom: parent.bottom
            }

            ComboBox{
                id: comboComBox
                textRole: "name"
                displayText: "COM Port Selection"
                model: ListModel {
                    id: comboComBoxModel
                }

                onCurrentIndexChanged: {
                    var selectedComPort = comboComBoxModel.get(comboComBox.currentIndex).name
                    console.log("user selected: ", selectedComPort)
                    myModel.openUART(selectedComPort);
                }
                width: 200
                // anchors.bottom: parent.bottom
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
                    mouseArea.enabled = true;  // Enable mouse area for drawing bounding box
                    comboCameraBox.enabled = false;
                }
                // anchors.right: parent.right
                // anchors.bottom: parent.bottom
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
                    comboCameraBox.enabled = true;
                }
                // anchors.right: parent.right
                // anchors.bottom: parent.bottom
                Material.background: "red" // Set the background color to a custom color
                Material.foreground: "white" // Set the text color to white
                visible: false
            }
        }

        Component.onCompleted: {
                // Populate the ComboBox with camera indexes from the model
                var cameraIndexes = myModel.getNumCams();// Call C++ method to get camera indexes
                console.log("We here in the on complete. Camera indexes: ", cameraIndexes);
                for (var i = 0; i < cameraIndexes; ++i) {
                   comboCameraBoxModel.append({ index: i, name: i.toString() })
                }
                var comPorts = myModel.getAvailComPorts()
                var size = myModel.getNumPorts();
                for (i = 0; i < size; i++) {
                    console.log(comPorts[i])
                    comboComBoxModel.append({index: i, name: comPorts[i]});
                }
            }

        Image {
            id: videoFrame
            width: 720 //parent.width
            height: 480
            source: "image://imageProvider/frame"  // Correct provider path
            visible: true
            fillMode: Image.PreserveAspectFit
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter

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
                comboCameraBox.enabled = true;
            }
        }

    }

    onWidthChanged: {
        mainText.font.pixelSize = width / 20  // Make the font size responsive
    }
}

