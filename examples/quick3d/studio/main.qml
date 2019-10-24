/****************************************************************************
**
** Copyright (C) 2019 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.12
import QtQuick.Window 2.12

import QtQuick3D 1.0
import QtQuick3D.Helpers 1.0

import QtGraphicalEffects 1.0

import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

import MouseArea3D 0.1

ApplicationWindow {
    id: window
    width: 640
    height: 480
    visible: true

    property alias overlayView: overlayView

    property color xAxisGizmoColor: Qt.rgba(1, 0, 0, 1)
    property color yAxisGizmoColor: Qt.rgba(0, 0, 1, 1)
    property color zAxisGizmoColor: Qt.rgba(0, 0.8, 0, 1)

    property Node nodeBeingManipulated: pot1

    property Node currentGizmoNode: rotationGizmo
    property Node currentGizmoAxisNode: null
    property bool inGizmoAxisDragMode: false

    property bool globalOrientation: globalControl.checked

    readonly property int kNoGizmoAxis: -1

    signal firstFrameReady

    Timer {
        // Work-around the fact that the projection matrix for the camera is not
        // calculated until the first frame is rendered, so any initial calls to
        // mapToViewport() etc will fail.
        interval: 1
        running: true
        onTriggered: firstFrameReady()
    }

    Node {
        id: mainScene

        PerspectiveCamera {
            id: camera1
            y: 200
            z: -50
            clipFar: 100000
        }

        OrthographicCamera {
            id: camera2
            y: 200
            z: -300
            clipFar: 100000
        }

        DirectionalLight {
            id: light
            y: 400
            color: Qt.rgba(0.4, 0.5, 0.0, 1.0)
            rotation: Qt.vector3d(60, 0, 0)
            brightness: 80
        }

        AxisHelper {
            id: axisGrid
            enableXZGrid: true
            enableAxisLines: false
        }

        Node {
            rotation: Qt.vector3d(0, 0, 0)
            x: 0
            y: 200
            z: 400
            Model {
                id: pot1
                objectName: "First pot"
                rotation: Qt.vector3d(0, 0, 45)
                pickable: true
                source: "meshes/Teapot.mesh"
                scale: Qt.vector3d(20, 20, 20)
                materials: DefaultMaterial {
                    diffuseColor: "salmon"
                }
            }
        }

        Model {
            id: pot2
            x: pot1.scenePosition.x + 50
            y: pot1.scenePosition.y
            z: pot1.scenePosition.z
            objectName: "Second pot"
            rotation: Qt.vector3d(0, 0, 0)
            pickable: true
            source: "meshes/Teapot.mesh"
            scale: Qt.vector3d(20, 20, 20)
            materials: DefaultMaterial {
                diffuseColor: "white"
            }

            Connections {
                target: pot1
                onSceneTransformChanged: {
                    pot2.rotationOrder = pot1.rotationOrder
                    pot2.rotation = pot1.sceneRotation
                    //pot2.rotation = pot2.parent.mapRotationFromScene(pot1.sceneRotation)
                    //pot2.lookAt(pot3.position, Qt.vector3d(0, 1, 0), Node.ParentSpace)
                }
            }

            //            Timer {
            //                running: true
            //                interval: 1000
            //                onTriggered: pot2.rotation = pot1.sceneRotation
            //            }
        }
    }


    Node {
        id: overlayScene

        PerspectiveCamera {
            id: overlayCamera1
            clipFar: camera1.clipFar
            position: camera1.position
            rotation: camera1.rotation
        }

        OrthographicCamera {
            id: overlayCamera2
            clipFar: camera2.clipFar
            position: camera2.position
            rotation: camera2.rotation
        }

        MoveGizmo {
            id: moveGizmo
            visible: currentGizmoNode == moveGizmo
            rotation: globalControl.checked ? Qt.vector3d(0, 0, 0) : window.nodeBeingManipulated.sceneRotation
            position: window.nodeBeingManipulated.scenePosition
            scale: Qt.vector3d(5, 5, 5)
        }

        RotationGizmo {
            id: rotationGizmo
            visible: currentGizmoNode == rotationGizmo
            position: window.nodeBeingManipulated.scenePosition
            rotation: globalControl.checked ? Qt.vector3d(0, 0, 0) : window.nodeBeingManipulated.sceneRotation
            rotationOrder: window.nodeBeingManipulated.rotationOrder
            scale: Qt.vector3d(100, 100, 100)
        }

//        AutoScaleHelper {
//            id: autoScale
//            view3D: overlayView
//            position: currentGizmoNode.positionInScene
//        }
    }

    RadialGradient {
        id: sceneBg
        anchors.fill: parent

        gradient: Gradient {
            GradientStop { position: 0.0; color: Qt.rgba(0.7, 0.7, 0.8, 1) }
            GradientStop { position: 0.5; color: Qt.rgba(0.5, 0.5, 0.5, 1) }
        }

        View3D {
            id: mainView
            anchors.fill: parent
            camera: perspectiveControl.checked ? camera1 : camera2
            importScene: mainScene
        }

        View3D {
            id: overlayView
            anchors.fill: parent
            camera: perspectiveControl.checked ? overlayCamera1 : overlayCamera2
            importScene: overlayScene
        }

        CameraGizmo {
            targetCamera: camera1
            anchors.right: parent.right
            width: 100
            height: 100
        }

        Overlay2D {
            id: overlayLabelPot1
            targetNode: pot1
            targetView: mainView
            offsetY: 100
            visible: showLabelsControl.checked

            Rectangle {
                color: "white"
                x: -width / 2
                y: -height
                width: pot1Text.width + 4
                height: pot1Text.height + 4
                border.width: 1
                Text {
                    id: pot1Text
                    text: pot1.objectName
                    anchors.centerIn: parent
                }
            }
        }

        Overlay2D {
            id: overlayLabelPot2
            targetNode: pot2
            targetView: mainView
            offsetY: 100
            visible: showLabelsControl.checked

            Rectangle {
                color: "white"
                x: -width / 2
                y: -height
                width: pot2Text.width + 4
                height: pot2Text.height + 4
                border.width: 1
                Text {
                    id: pot2Text
                    text: pot2.objectName
                    anchors.centerIn: parent
                }
            }
        }

        WasdController {
            id: wasd
            controlledObject: mainView.camera
            acceptedButtons: Qt.RightButton
        }

        HoverHandler {
            onPointChanged: {
                // Check what is under the mouse in the overlay view
                var mousePressed = point.pressedButtons === Qt.LeftButton;

                if (!mousePressed)
                    inGizmoAxisDragMode = false

                if (inGizmoAxisDragMode) {
                    currentGizmoAxisNode.continueDrag(point.position)
                } else {
                    // Start a new drag?
                    var pickResult = overlayView.pick(point.position.x, point.position.y)
                    var nodeUnderMouse = pickResult.objectHit
                    if (!nodeUnderMouse) {
                        currentGizmoAxisNode = null
                    } else {
                        var gizmoPart = nodeUnderMouse.gizmoAxisRoot
                        if (!gizmoPart)
                            return;
                        currentGizmoAxisNode = gizmoPart
                        if (mousePressed) {
                            inGizmoAxisDragMode = true
                            gizmoPart.startDrag(point.position)
                        }
                    }
                }
            }
        }

        TapHandler {
            onTapped: {
                // Check if the user selected a new object in the scene.
                // If so, we change nodeBeingManipulated, which will also move the gizmo
                var pickResult = mainView.pick(point.position.x, point.position.y)
                if (pickResult.objectHit)
                    nodeBeingManipulated = pickResult.objectHit
            }
        }
    }

    Item {
        id: menu
        anchors.fill: parent

        ColumnLayout {
            anchors.fill: parent

            CheckBox {
                id: globalControl
                checked: true
                onCheckedChanged: wasd.forceActiveFocus()
                Text {
                    anchors.left: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    text: qsTr("Use global orientation")
                }
            }

            CheckBox {
                id: perspectiveControl
                checked: true
                onCheckedChanged: {
                    wasd.forceActiveFocus()
                }
                Text {
                    anchors.left: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    text: qsTr("Use perspective")
                }
            }

            CheckBox {
                id: autoScaleControl
                checked: true
                onCheckedChanged: wasd.forceActiveFocus()
                Text {
                    anchors.left: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    text: qsTr("Use fixed-sized gizmo")
                }
            }

            CheckBox {
                id: showLabelsControl
                checked: true
                onCheckedChanged: wasd.forceActiveFocus()
                Text {
                    anchors.left: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    text: qsTr("Show labels")
                }
            }

            Button {
                text: "Reset"
                onClicked: {
                    pot1.rotation = Qt.vector3d(0, 0, 0)
                    wasd.forceActiveFocus()
                }
            }

            Button {
                text: "Cam front"
                onClicked: {
                    camera1.rotation = Qt.vector3d(0, 0, 0)
                    camera1.position = Qt.vector3d(0, 200, -600)
                    wasd.forceActiveFocus()
                }
            }

            Button {
                text: "Cam side"
                onClicked: {
                    camera1.rotation = Qt.vector3d(0, -90, 0)
                    camera1.position = Qt.vector3d(600, 200, 200)
                    wasd.forceActiveFocus()
                }
            }

            Button {
                text: "Cam top"
                onClicked: {
                    camera1.rotation = Qt.vector3d(90, 0, 0)
                    camera1.position = Qt.vector3d(0, 600, 200)
                    wasd.forceActiveFocus()
                }
            }

            Button {
                text: "Rotate Y"
                onClicked: {
                    nodeBeingManipulated.rotate(15, Qt.vector3d(0, 1, 0));
                    wasd.forceActiveFocus()
                }
            }

            Button {
                text: "Rotate Z"
                onClicked: {
                    nodeBeingManipulated.rotate(15, Qt.vector3d(0, 0, 1));
                    wasd.forceActiveFocus()
                }
            }

            Item {
                Layout.fillHeight: true
            }
        }
    }

    Text {
        text: "Camera: W,A,S,D,R,F,right mouse drag"
        anchors.bottom: parent.bottom
    }

}
