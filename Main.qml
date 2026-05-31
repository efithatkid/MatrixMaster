import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    id: window
    width: 360
    height: 750
    visible: true
    title: "Linear Algebra - Rahman"

    property int matrixSize: parseInt(sizeSelector.currentText)

    Flickable {
        anchors.fill: parent
        contentHeight: contentLayout.height + 40
        clip: true

        ColumnLayout {
            id: contentLayout
            width: parent.width - 32
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 16

            Text {
                text: "Rahman Taleshi — AUSMT"
                font.bold: true
                font.pointSize: 20
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 20
            }

            // Size Selector (n)
            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                spacing: 10
                Text { text: "Select Matrix Size (n):"; font.pointSize: 14; font.bold: true }
                ComboBox {
                    id: sizeSelector
                    model: ["2", "3", "4", "5"]
                    currentIndex: 1 // Defaults to 3x3
                }
            }

            // Matrix A Input Grid
            Text { text: "Matrix A Elements:"; font.bold: true; font.pointSize: 14 }

            Grid {
                id: matrixAGrid
                columns: matrixSize
                spacing: 8
                Layout.alignment: Qt.AlignHCenter

                Repeater {
                    id: matrixARepeater
                    model: matrixSize * matrixSize
                    TextField {
                        width: (window.width - 64) / matrixSize
                        placeholderText: "0"
                        horizontalAlignment: Text.AlignHCenter
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                    }
                }
            }

            // Vector B Input Grid
            Text { text: "Vector B (For Systems):"; font.bold: true; font.pointSize: 14 }

            Grid {
                id: vectorBGrid
                columns: matrixSize
                spacing: 8
                Layout.alignment: Qt.AlignHCenter

                Repeater {
                    id: vectorBRepeater
                    model: matrixSize
                    TextField {
                        width: (window.width - 64) / matrixSize
                        placeholderText: "0"
                        horizontalAlignment: Text.AlignHCenter
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                    }
                }
            }

            // Action Buttons using Capitalized Alias
            Button {
                text: "Solve System"
                Layout.fillWidth: true
                onClicked: {
                    var res = MatrixSolver.solveSystem(getMatrixAData(), getVectorBData(), matrixSize)
                    resultText.text = res.length ? "Result Vector: \n" + JSON.stringify(res) : "No unique solution."
                }
            }

            Button {
                text: "Find Inverse"
                Layout.fillWidth: true
                onClicked: {
                    var res = MatrixSolver.findInverse(getMatrixAData(), matrixSize)
                    resultText.text = res.length ? "Inverse Matrix (Flat): \n" + JSON.stringify(res) : "Not Invertible (Det = 0)"
                }
            }

            Button {
                text: "Calculate Determinant"
                Layout.fillWidth: true
                onClicked: {
                    var det = MatrixSolver.calculateDeterminant(getMatrixAData(), matrixSize)
                    resultText.text = "Determinant = " + det
                }
            }

            // Output Display
            Rectangle {
                Layout.fillWidth: true
                height: 100
                color: "#f5f5f5"
                border.color: "#cccccc"
                radius: 6

                ScrollView {
                    anchors.fill: parent
                    padding: 10
                    Text {
                        id: resultText
                        text: "Results will appear here..."
                        font.pointSize: 12
                        wrapMode: Text.Wrap
                        width: parent.width
                    }
                }
            }
        }
    }

    function getMatrixAData() {
        var data = [];
        for (var i = 0; i < matrixSize * matrixSize; i++) {
            var item = matrixARepeater.itemAt(i);
            var val = item ? parseFloat(item.text) : 0.0;
            data.push(isNaN(val) ? 0.0 : val);
        }
        return data;
    }

    function getVectorBData() {
        var data = [];
        for (var i = 0; i < matrixSize; i++) {
            var item = vectorBRepeater.itemAt(i);
            var val = item ? parseFloat(item.text) : 0.0;
            data.push(isNaN(val) ? 0.0 : val);
        }
        return data;
    }
}