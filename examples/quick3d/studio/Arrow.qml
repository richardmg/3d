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

import QtQuick 2.0
import QtQuick3D 1.0

Model {
    id: arrow
    rotationOrder: Node.XYZr
    source: "meshes/Arrow.mesh"
    pickable: true

    property Node gizmoRoot
    property Node gizmoAxisRoot: arrow
    property int axis: kNoGizmoAxis

    property color color: "white"

    property var _pointerPosPressed
    property point _mouseStartPos
    property var _targetStartPos

    property int _draggingOnBackside

    materials: DefaultMaterial {
        id: material
        emissiveColor: currentGizmoAxisNode == arrow ? Qt.lighter(color) : color
        lighting: DefaultMaterial.NoLighting
    }

    function startDrag(mousePos)
    {
        _mouseStartPos = mousePos
        var sp = nodeBeingManipulated.position
        _targetStartPos = Qt.vector3d(sp.x, sp.y, sp.z)

        var relCamPos = overlayView.camera.mapPositionToNode(arrow, Qt.vector3d(0, 0, 0));
        _draggingOnBackside = relCamPos.z > 0 ? -1 : 1
        if (nodeBeingManipulated.orientation === Node.RightHanded)
            _draggingOnBackside *= -1;

        print(_draggingOnBackside)
    }

    function continueDrag(mousePos)
    {
        var deltaX = mousePos.x - _mouseStartPos.x
        var deltaY = mousePos.y - _mouseStartPos.y
        deltaY *= -1 // Convert

//        deltaX *= _draggingOnBackside
//        deltaY *= _draggingOnBackside

        var newPos = Qt.vector3d(_targetStartPos.x + deltaX, 0, 0)

//        print(_targetStartPos.y + deltaY, newPos)

//        var posInParent = nodeBeingManipulated.parent.mapPositionFromScene(newScenePos)
        nodeBeingManipulated.position = newPos
    }


//    function handlePressed(mouseArea, pointerPosition)
//    {
//        if (!targetNode)
//            return;

//        var maskedPosition = Qt.vector3d(pointerPosition.x, 0, 0)
//        _pointerPosPressed = mouseArea.mapPositionToScene(maskedPosition)
//        var sp = targetNode.scenePosition
//        _targetStartPos = Qt.vector3d(sp.x, sp.y, sp.z);
//    }

//    function handleDragged(mouseArea, pointerPosition)
//    {
//        if (!targetNode)
//            return;

//        var maskedPosition = Qt.vector3d(pointerPosition.x, 0, 0)
//        var scenePointerPos = mouseArea.mapPositionToScene(maskedPosition)
//        var sceneRelativeDistance = Qt.vector3d(
//                    scenePointerPos.x - _pointerPosPressed.x,
//                    scenePointerPos.y - _pointerPosPressed.y,
//                    scenePointerPos.z - _pointerPosPressed.z)

//        var newScenePos = Qt.vector3d(
//                    _targetStartPos.x + sceneRelativeDistance.x,
//                    _targetStartPos.y + sceneRelativeDistance.y,
//                    _targetStartPos.z + sceneRelativeDistance.z)

//        var posInParent = targetNode.parent.mapPositionFromScene(newScenePos)
//        targetNode.position = posInParent
//    }

}

