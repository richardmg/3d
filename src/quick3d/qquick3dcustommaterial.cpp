/****************************************************************************
**
** Copyright (C) 2019 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Quick 3D.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qquick3dcustommaterial_p.h"
#include <QtQuick3DRuntimeRender/private/qssgrendercustommaterial_p.h>
#include <QtQuick3DRuntimeRender/private/qssgrendercontextcore_p.h>

#include "qquick3dobject_p_p.h"
#include "qquick3dviewport_p.h"

Q_DECLARE_OPAQUE_POINTER(QQuick3DCustomMaterialTextureInput)

QT_BEGIN_NAMESPACE

template <QVariant::Type>
struct ShaderType
{
};

template<>
struct ShaderType<QVariant::Double>
{
    static constexpr QSSGRenderShaderDataType type() { return QSSGRenderShaderDataType::Float; }
    static QByteArray name() { return QByteArrayLiteral("float"); }
};

template<>
struct ShaderType<QVariant::Bool>
{
    static constexpr QSSGRenderShaderDataType type() { return QSSGRenderShaderDataType::Boolean; }
    static QByteArray name() { return QByteArrayLiteral("bool"); }
};

template<>
struct ShaderType<QVariant::Int>
{
    static constexpr QSSGRenderShaderDataType type() { return QSSGRenderShaderDataType::Integer; }
    static QByteArray name() { return QByteArrayLiteral("int"); }
};

template<>
struct ShaderType<QVariant::Vector2D>
{
    static constexpr QSSGRenderShaderDataType type() { return QSSGRenderShaderDataType::Vec2; }
    static QByteArray name() { return QByteArrayLiteral("vec2"); }
};

template<>
struct ShaderType<QVariant::Vector3D>
{
    static constexpr QSSGRenderShaderDataType type() { return QSSGRenderShaderDataType::Vec3; }
    static QByteArray name() { return QByteArrayLiteral("vec3"); }
};

template<>
struct ShaderType<QVariant::Vector4D>
{
    static constexpr QSSGRenderShaderDataType type() { return QSSGRenderShaderDataType::Vec4; }
    static QByteArray name() { return QByteArrayLiteral("vec4"); }
};

/*!
    \qmltype CustomMaterial
    \inqmlmodule QtQuick3D.Materials
    \brief Base component for creating custom materials used to shade models.
*/



/*!
    \qmltype Shader
    \inqmlmodule QtQuick3D.Materials
    \brief Container component for defining shader code used by CustomMaterials.
*/

/*!
    \qmltype ShaderInfo
    \inqmlmodule QtQuick3D.Materials
    \brief Defines basic information about custom shader code for CustomMaterials.
*/

/*!
    \qmltype TextureInput
    \inqmlmodule QtQuick3D.Materials
    \brief Defines a texture channel for a Custom Material.
*/

/*!
 * \qmlproperty Texture TextureInput::image
 *
 * The property defines the source Texture
 *
 */

/*!
 * \qmlproperty enumeration TextureInput::type
 *
 * The property defines what channel the texture is being used for.
 *
 * \list
 * \li TextureInput.Unknown
 * \li TextureInput.Diffuse
 * \li TextureInput.Specular
 * \li TextureInput.Environment
 * \li TextureInput.Bump
 * \li TextureInput.Normal
 * \li TextureInput.Displace
 * \li TextureInput.Emissive
 * \li TextureInput.Anisotropy
 * \li TextureInput.Translucent
 * \li TextureInput.LightmapIndirect
 * \li TextureInput.LightmapRadiosity
 * \li TextureInput.LightmapShadow
 * \endlist
 *
 */

/*!
 * \qmlproperty bool TextureInput::enabled
 *
 * The property determines if this channel is enabled.
 *
 */

/*!
    \qmltype Pass
    \inqmlmodule QtQuick3D.Materials
    \brief Defines a pass in the Custom Material API.
*/

/*!
    \qmltype Command
    \inqmlmodule QtQuick3D.Materials
    \brief Defines a command to be performed in a pass of a CustomMaterial.
*/

/*!
    \qmltype BufferInput
    \inqmlmodule QtQuick3D.Materials
    \brief Defines an input buffer to be used for a pass of a CustomMaterial.
*/

/*!
    \qmltype BufferBlit
    \inqmlmodule QtQuick3D.Materials
    \brief Defines a copy operation between two buffers in a pass of a CustomMaterial.
*/

/*!
    \qmltype Blending
    \inqmlmodule QtQuick3D.Materials
    \brief Defines the blending state in a pass of a CustomMaterial.
*/

/*!
    \qmltype Buffer
    \inqmlmodule QtQuick3D.Materials
    \brief Defines a buffer to be used for a pass of a CustomMaterial.
*/

/*!
    \qmltype RenderState
    \inqmlmodule QtQuick3D.Materials
    \brief Defines the render state to be disabled in a pass of a CustomMaterial.
*/

QQuick3DCustomMaterialBuffer::TextureFormat QQuick3DCustomMaterialBuffer::format() const
{
    return mapRenderTextureFormat(command.m_format.format);
}

void QQuick3DCustomMaterialBuffer::setFormat(TextureFormat format)
{
    command.m_format = mapTextureFormat(format);
}

QQuick3DCustomMaterialBuffer::TextureFormat QQuick3DCustomMaterialBuffer::mapRenderTextureFormat(
        QSSGRenderTextureFormat::Format fmt)
{
    switch (fmt) {
    case QSSGRenderTextureFormat::R8: return TextureFormat::R8;
    case QSSGRenderTextureFormat::R16: return TextureFormat::R16;
    case QSSGRenderTextureFormat::R16F: return TextureFormat::R16F;
    case QSSGRenderTextureFormat::R32I: return TextureFormat::R32I;
    case QSSGRenderTextureFormat::R32UI: return TextureFormat::R32UI;
    case QSSGRenderTextureFormat::R32F: return TextureFormat::R32F;
    case QSSGRenderTextureFormat::RG8: return TextureFormat::RG8;
    case QSSGRenderTextureFormat::RGBA8: return TextureFormat::RGBA8;
    case QSSGRenderTextureFormat::RGB8: return TextureFormat::RGB8;
    case QSSGRenderTextureFormat::SRGB8: return TextureFormat::SRGB8;
    case QSSGRenderTextureFormat::SRGB8A8: return TextureFormat::SRGB8A8;
    case QSSGRenderTextureFormat::RGB565: return TextureFormat::RGB565;
    case QSSGRenderTextureFormat::RGBA16F: return TextureFormat::RGBA16F;
    case QSSGRenderTextureFormat::RG16F: return TextureFormat::RG16F;
    case QSSGRenderTextureFormat::RG32F: return TextureFormat::RG32F;
    case QSSGRenderTextureFormat::RGB32F: return TextureFormat::RGB32F;
    case QSSGRenderTextureFormat::RGBA32F: return TextureFormat::RGBA32F;
    case QSSGRenderTextureFormat::R11G11B10: return TextureFormat::R11G11B10;
    case QSSGRenderTextureFormat::RGB9E5: return TextureFormat::RGB9E5;
    case QSSGRenderTextureFormat::Depth16: return TextureFormat::Depth16;
    case QSSGRenderTextureFormat::Depth24: return TextureFormat::Depth24;
    case QSSGRenderTextureFormat::Depth32: return TextureFormat::Depth32;
    case QSSGRenderTextureFormat::Depth24Stencil8: return TextureFormat::Depth24Stencil8;
    default:
        break;
    }
    return TextureFormat::Unknown;
}

QSSGRenderTextureFormat::Format QQuick3DCustomMaterialBuffer::mapTextureFormat(
        QQuick3DCustomMaterialBuffer::TextureFormat fmt)
{
    switch (fmt) {
    case TextureFormat::R8: return QSSGRenderTextureFormat::R8;
    case TextureFormat::R16: return QSSGRenderTextureFormat::R16;
    case TextureFormat::R16F: return QSSGRenderTextureFormat::R16F;
    case TextureFormat::R32I: return QSSGRenderTextureFormat::R32I;
    case TextureFormat::R32UI: return QSSGRenderTextureFormat::R32UI;
    case TextureFormat::R32F: return QSSGRenderTextureFormat::R32F;
    case TextureFormat::RG8: return QSSGRenderTextureFormat::RG8;
    case TextureFormat::RGBA8: return QSSGRenderTextureFormat::RGBA8;
    case TextureFormat::RGB8: return QSSGRenderTextureFormat::RGB8;
    case TextureFormat::SRGB8: return QSSGRenderTextureFormat::SRGB8;
    case TextureFormat::SRGB8A8: return QSSGRenderTextureFormat::SRGB8A8;
    case TextureFormat::RGB565: return QSSGRenderTextureFormat::RGB565;
    case TextureFormat::RGBA16F: return QSSGRenderTextureFormat::RGBA16F;
    case TextureFormat::RG16F: return QSSGRenderTextureFormat::RG16F;
    case TextureFormat::RG32F: return QSSGRenderTextureFormat::RG32F;
    case TextureFormat::RGB32F: return QSSGRenderTextureFormat::RGB32F;
    case TextureFormat::RGBA32F: return QSSGRenderTextureFormat::RGBA32F;
    case TextureFormat::R11G11B10: return QSSGRenderTextureFormat::R11G11B10;
    case TextureFormat::RGB9E5: return QSSGRenderTextureFormat::RGB9E5;
    case TextureFormat::Depth16: return QSSGRenderTextureFormat::Depth16;
    case TextureFormat::Depth24: return QSSGRenderTextureFormat::Depth24;
    case TextureFormat::Depth32: return QSSGRenderTextureFormat::Depth32;
    case TextureFormat::Depth24Stencil8: return QSSGRenderTextureFormat::Depth24Stencil8;
    default:
        break;
    }
    return QSSGRenderTextureFormat::Unknown;
}

QQuick3DCustomMaterial::QQuick3DCustomMaterial() {}

QQuick3DCustomMaterial::~QQuick3DCustomMaterial() {}

QQuick3DObject::Type QQuick3DCustomMaterial::type() const
{
    return QQuick3DObject::CustomMaterial;
}

bool QQuick3DCustomMaterial::hasTransparency() const
{
    return m_hasTransparency;
}

bool QQuick3DCustomMaterial::hasRefraction() const
{
    return m_hasRefraction;
}

QQuick3DCustomMaterialShaderInfo *QQuick3DCustomMaterial::shaderInfo() const
{
    return m_shaderInfo;
}

QQmlListProperty<QQuick3DCustomMaterialRenderPass> QQuick3DCustomMaterial::passes()
{
    return QQmlListProperty<QQuick3DCustomMaterialRenderPass>(this,
                                                            nullptr,
                                                            QQuick3DCustomMaterial::qmlAppendPass,
                                                            QQuick3DCustomMaterial::qmlPassCount,
                                                            QQuick3DCustomMaterial::qmlPassAt,
                                                            nullptr);
}

bool QQuick3DCustomMaterial::alwaysDirty() const
{
    return m_alwaysDirty;
}

void QQuick3DCustomMaterial::setHasTransparency(bool hasTransparency)
{
    if (m_hasTransparency == hasTransparency)
        return;

    m_hasTransparency = hasTransparency;
    emit hasTransparencyChanged(m_hasTransparency);
}

void QQuick3DCustomMaterial::setHasRefraction(bool hasRefraction)
{
    if (m_hasRefraction == hasRefraction)
        return;

    m_hasRefraction = hasRefraction;
    emit hasRefractionChanged(m_hasRefraction);
}

void QQuick3DCustomMaterial::setShaderInfo(QQuick3DCustomMaterialShaderInfo *shaderInfo)
{
    m_shaderInfo = shaderInfo;
}

void QQuick3DCustomMaterial::setAlwaysDirty(bool alwaysDirty)
{
    if (m_alwaysDirty == alwaysDirty)
        return;

    m_alwaysDirty = alwaysDirty;
    emit alwaysDirtyChanged(m_alwaysDirty);
}

QSSGRenderGraphObject *QQuick3DCustomMaterial::updateSpatialNode(QSSGRenderGraphObject *node)
{
    static const auto updateShaderPrefix = [](QByteArray &shaderPrefix, const QByteArray &name) {
        const char *filter = "linear";
        const char *clamp = "clamp";
        // Output macro so we can change the set of variables used for this
        // independent of the
        // meta data system.
        shaderPrefix.append("SNAPPER_SAMPLER2D(");
        shaderPrefix.append(name);
        shaderPrefix.append(", ");
        shaderPrefix.append(name);
        shaderPrefix.append(", ");
        shaderPrefix.append(filter);
        shaderPrefix.append(", ");
        shaderPrefix.append(clamp);
        shaderPrefix.append(", ");
        shaderPrefix.append("false )\n");
    };

    static const auto appendShaderUniform = [](const QByteArray &type, const QByteArray &name, QByteArray *shaderPrefix) {
        shaderPrefix->append(QByteArrayLiteral("uniform ") + type + " " + name + ";\n");
    };

    static const auto resolveShader = [](const QByteArray &shader) -> QByteArray {
        int offset = -1;
        if (shader.startsWith("qrc:/"))
            offset = 3;
        else if (shader.startsWith("file:/"))
            offset = 6;
        else if (shader.startsWith(":/"))
            offset = 0;

        QString path;
        if (offset == -1) {
            QUrl u(QString::fromUtf8(shader));
            if (u.isLocalFile())
                path = u.toLocalFile();
        }

        if (offset == -1 && path.isEmpty())
            path = QString::fromLatin1(":/") + QString::fromLocal8Bit(shader);
        else
            path = QString::fromLocal8Bit(shader.constData() + offset);

        QFile f(path);
        if (f.open(QIODevice::ReadOnly | QIODevice::Text))
            return f.readAll();

        return shader;
    };

    static const auto mergeShaderCode = [](const QByteArray &shared, const QByteArray &vertex, const QByteArray &geometry, const QByteArray &fragment) {
        QByteArray shaderCode;
            // Shared
            if (!shared.isEmpty())
                shaderCode.append(shared);

            // Vetex
            shaderCode.append(QByteArrayLiteral("\n#ifdef VERTEX_SHADER\n"));
            if (!vertex.isEmpty())
                shaderCode.append(vertex);
            else
                shaderCode.append(QByteArrayLiteral("void vert(){}"));
            shaderCode.append(QByteArrayLiteral("\n#endif\n"));

            // Geometry
            if (!geometry.isEmpty()) {
                shaderCode.append(QByteArrayLiteral("\n#ifdef USER_GEOMETRY_SHADER\n"));
                shaderCode.append(geometry);
                shaderCode.append(QByteArrayLiteral("\n#endif\n"));
            }

            // Fragment
            shaderCode.append(QByteArrayLiteral("\n#ifdef FRAGMENT_SHADER\n"));
            if (!fragment.isEmpty())
                shaderCode.append(fragment);
            else
                shaderCode.append(QByteArrayLiteral("void frag(){}"));
            shaderCode.append(QByteArrayLiteral("\n#endif\n"));

            return shaderCode;
    };


    // Sanity check(s)
    if (!m_shaderInfo || !m_shaderInfo->isValid()) {
        qWarning("ShaderInfo is not valid!");
        return node;
    }

    // Find the parent view
    QObject *p = this;
    QQuick3DViewport *view = nullptr;
    while (p != nullptr && view == nullptr) {
        p = p->parent();
        if ((view = qobject_cast<QQuick3DViewport *>(p)))
            break;
    }

    Q_ASSERT(view);
    QSSGRenderContextInterface::QSSGRenderContextInterfacePtr renderContext = QSSGRenderContextInterface::getRenderContextInterface(quintptr(view->window()));

    QSSGRenderCustomMaterial *customMaterial = static_cast<QSSGRenderCustomMaterial *>(node);
    if (!customMaterial) {
        customMaterial = new QSSGRenderCustomMaterial;
        customMaterial->m_shaderKeyValues = static_cast<QSSGRenderCustomMaterial::MaterialShaderKeyFlags>(m_shaderInfo->shaderKey);
        customMaterial->className = metaObject()->className();
        customMaterial->m_alwaysDirty = m_alwaysDirty;
        customMaterial->m_hasTransparency = m_hasTransparency;
        customMaterial->m_hasRefraction = m_hasRefraction;

        // Shader info
        auto &shaderInfo = customMaterial->shaderInfo;
        shaderInfo.type = m_shaderInfo->type;
        shaderInfo.version = m_shaderInfo->version;
        shaderInfo.shaderPrefix = m_shaderInfo->shaderPrefix;

        QMetaMethod propertyDirtyMethod;
        const int idx = metaObject()->indexOfSlot("onPropertyDirty()");
        if (idx != -1)
            propertyDirtyMethod = metaObject()->method(idx);

        // Properties
        const int propCount = metaObject()->propertyCount();
        const int propOffset = metaObject()->propertyOffset();
        QVector<QMetaProperty> userProperties;
        for (int i = propOffset; i != propCount; ++i) {
            const auto property = metaObject()->property(i);
            if (Q_UNLIKELY(!property.isValid()))
                continue;

            // Track the property changes
            if (property.hasNotifySignal() && propertyDirtyMethod.isValid())
                connect(this, property.notifySignal(), this, propertyDirtyMethod);

            if (property.type() == QVariant::Double) {
                appendShaderUniform(ShaderType<QVariant::Double>::name(), property.name(), &shaderInfo.shaderPrefix);
                customMaterial->properties.push_back({ property.name(), property.read(this), ShaderType<QVariant::Double>::type(), i});
            } else if (property.type() == QVariant::Bool) {
                appendShaderUniform(ShaderType<QVariant::Bool>::name(), property.name(), &shaderInfo.shaderPrefix);
                customMaterial->properties.push_back({ property.name(), property.read(this), ShaderType<QVariant::Bool>::type(), i});
            } else if (property.type() == QVariant::Vector2D) {
                appendShaderUniform(ShaderType<QVariant::Vector2D>::name(), property.name(), &shaderInfo.shaderPrefix);
                customMaterial->properties.push_back({ property.name(), property.read(this), ShaderType<QVariant::Vector2D>::type(), i});
            } else if (property.type() == QVariant::Vector3D) {
                appendShaderUniform(ShaderType<QVariant::Vector3D>::name(), property.name(), &shaderInfo.shaderPrefix);
                customMaterial->properties.push_back({ property.name(), property.read(this), ShaderType<QVariant::Vector3D>::type(), i});
            } else if (property.type() == QVariant::Vector4D) {
                appendShaderUniform(ShaderType<QVariant::Vector4D>::name(), property.name(), &shaderInfo.shaderPrefix);
                customMaterial->properties.push_back({ property.name(), property.read(this), ShaderType<QVariant::Vector4D>::type(), i});
            } else if (property.type() == QVariant::Int) {
                appendShaderUniform(ShaderType<QVariant::Int>::name(), property.name(), &shaderInfo.shaderPrefix);
                customMaterial->properties.push_back({ property.name(), property.read(this), ShaderType<QVariant::Int>::type(), i});
            } else if (property.type() == QVariant::UserType) {
                if (property.userType() == qMetaTypeId<QQuick3DCustomMaterialTextureInput *>())
                    userProperties.push_back(property);
            } else {
                Q_ASSERT(0);
            }
        }

        // Textures
        for (const auto &userProperty : qAsConst(userProperties)) {
            QSSGRenderCustomMaterial::TextureProperty textureData;
            QQuick3DCustomMaterialTextureInput *texture = userProperty.read(this).value<QQuick3DCustomMaterialTextureInput *>();
            const QByteArray &name = userProperty.name();
            if (name.isEmpty()) // Warnings here will just drown in the shader error messages
                continue;
            QQuick3DTexture *tex = texture->texture(); //
            connect(texture, &QQuick3DCustomMaterialTextureInput::textureDirty, this, &QQuick3DCustomMaterial::onTextureDirty);
            textureData.name = name;
            if (texture->enabled)
                textureData.texImage = tex->getRenderImage();
            textureData.shaderDataType = QSSGRenderShaderDataType::Texture2D;
            textureData.clampType = tex->horizontalTiling() == QQuick3DTexture::Repeat ? QSSGRenderTextureCoordOp::Repeat
                                                                                     : (tex->horizontalTiling() == QQuick3DTexture::ClampToEdge) ? QSSGRenderTextureCoordOp::ClampToEdge
                                                                                                                                               : QSSGRenderTextureCoordOp::MirroredRepeat;
            updateShaderPrefix(shaderInfo.shaderPrefix, textureData.name);
            customMaterial->textureProperties.push_back(textureData);
        }

        QByteArray &shared = shaderInfo.shaderPrefix;
        QByteArray vertex, geometry, fragment, shaderCode;
        if (!m_passes.isEmpty()) {
            for (const auto &pass : qAsConst(m_passes)) {
                QQuick3DCustomMaterialShader *sharedShader = pass->m_shaders.at(int(QQuick3DCustomMaterialShader::Stage::Shared));
                QQuick3DCustomMaterialShader *vertShader = pass->m_shaders.at(int(QQuick3DCustomMaterialShader::Stage::Vertex));
                QQuick3DCustomMaterialShader *fragShader = pass->m_shaders.at(int(QQuick3DCustomMaterialShader::Stage::Fragment));
                QQuick3DCustomMaterialShader *geomShader = pass->m_shaders.at(int(QQuick3DCustomMaterialShader::Stage::Geometry));
                if (!sharedShader && !vertShader && !fragShader && !geomShader) {
                    qWarning("Pass with no shader attatched!");
                    continue;
                }

                // Build up shader code
                const QByteArray &shaderName = fragShader ? fragShader->shader : vertShader->shader;
                Q_ASSERT(!shaderName.isEmpty());

                if (sharedShader)
                    shared += resolveShader(sharedShader->shader);
                if (vertShader)
                    vertex = resolveShader(vertShader->shader);
                if (fragShader)
                    fragment = resolveShader(fragShader->shader);
                if (geomShader)
                    geometry = resolveShader(geomShader->shader);

                shaderCode = mergeShaderCode(shared, vertex, geometry, fragment);

                // Bind shader
                customMaterial->commands.push_back(new dynamic::QSSGBindShader(shaderName));
                customMaterial->commands.push_back(new dynamic::QSSGApplyInstanceValue());

                // Buffers
                QQuick3DCustomMaterialBuffer *outputBuffer = pass->outputBuffer;
                if (outputBuffer) {
                    const QByteArray &outBufferName = outputBuffer->name;
                    Q_ASSERT(!outBufferName.isEmpty());
                    // Allocate buffer command
                    customMaterial->commands.push_back(outputBuffer->getCommand());
                    // bind buffer
                    customMaterial->commands.push_back(new dynamic::QSSGBindBuffer(outBufferName, true));
                } else {
                    customMaterial->commands.push_back(new dynamic::QSSGBindTarget(QSSGRenderTextureFormat::RGBA8));
                }

                // Other commands (BufferInput, Blending ... )
                const auto &extraCommands = pass->m_commands;
                for (const auto &command : extraCommands) {
                    const int bufferCount = command->bufferCount();
                    for (int i = 0; i != bufferCount; ++i)
                        customMaterial->commands.push_back(command->bufferAt(i)->getCommand());
                    customMaterial->commands.push_back(command->getCommand());
                }

                // ... and finaly the render command (TODO: indirect/or not?)
                customMaterial->commands.push_back(new dynamic::QSSGRender(false));

                renderContext->customMaterialSystem()->setMaterialClassShader(shaderName, shaderInfo.type, shaderInfo.version, shaderCode, false, false);
            }
        }
    }

    QQuick3DMaterial::updateSpatialNode(customMaterial);

    if (m_dirtyAttributes & Dirty::PropertyDirty) {
        for (const auto &prop : qAsConst(customMaterial->properties)) {
            auto p = metaObject()->property(prop.pid);
            if (Q_LIKELY(p.isValid()))
                prop.value = p.read(this);
        }
    }

    if (m_dirtyAttributes & Dirty::TextureDirty) {
        // TODO:
    }

    return customMaterial;
}

void QQuick3DCustomMaterial::onPropertyDirty()
{
    markDirty(Dirty::PropertyDirty);
    update();
}

void QQuick3DCustomMaterial::onTextureDirty(QQuick3DCustomMaterialTextureInput *texture)
{
    Q_UNUSED(texture)
    markDirty(Dirty::TextureDirty);
    update();
}

void QQuick3DCustomMaterial::qmlAppendPass(QQmlListProperty<QQuick3DCustomMaterialRenderPass> *list, QQuick3DCustomMaterialRenderPass *pass)
{
    if (!pass)
        return;

    QQuick3DCustomMaterial *that = qobject_cast<QQuick3DCustomMaterial *>(list->object);
    that->m_passes.push_back(pass);
}

QQuick3DCustomMaterialRenderPass *QQuick3DCustomMaterial::qmlPassAt(QQmlListProperty<QQuick3DCustomMaterialRenderPass> *list, int index)
{
    QQuick3DCustomMaterial *that = qobject_cast<QQuick3DCustomMaterial *>(list->object);
    return that->m_passes.at(index);
}

int QQuick3DCustomMaterial::qmlPassCount(QQmlListProperty<QQuick3DCustomMaterialRenderPass> *list)
{
    QQuick3DCustomMaterial *that = qobject_cast<QQuick3DCustomMaterial *>(list->object);
    return that->m_passes.count();
}

void QQuick3DCustomMaterialRenderPass::qmlAppendCommand(QQmlListProperty<QQuick3DCustomMaterialRenderCommand> *list, QQuick3DCustomMaterialRenderCommand *command)
{
    if (!command)
        return;

    QQuick3DCustomMaterialRenderPass *that = qobject_cast<QQuick3DCustomMaterialRenderPass *>(list->object);
    that->m_commands.push_back(command);
}

QQuick3DCustomMaterialRenderCommand *QQuick3DCustomMaterialRenderPass::qmlCommandAt(QQmlListProperty<QQuick3DCustomMaterialRenderCommand> *list, int index)
{
    QQuick3DCustomMaterialRenderPass *that = qobject_cast<QQuick3DCustomMaterialRenderPass *>(list->object);
    return that->m_commands.at(index);
}

int QQuick3DCustomMaterialRenderPass::qmlCommandCount(QQmlListProperty<QQuick3DCustomMaterialRenderCommand> *list)
{
    QQuick3DCustomMaterialRenderPass *that = qobject_cast<QQuick3DCustomMaterialRenderPass *>(list->object);
    return that->m_commands.count();
}

QQmlListProperty<QQuick3DCustomMaterialRenderCommand> QQuick3DCustomMaterialRenderPass::commands()
{
    return QQmlListProperty<QQuick3DCustomMaterialRenderCommand>(this,
                                                               nullptr,
                                                               QQuick3DCustomMaterialRenderPass::qmlAppendCommand,
                                                               QQuick3DCustomMaterialRenderPass::qmlCommandCount,
                                                               QQuick3DCustomMaterialRenderPass::qmlCommandAt,
                                                               nullptr);
}

void QQuick3DCustomMaterialRenderPass::qmlAppendShader(QQmlListProperty<QQuick3DCustomMaterialShader> *list, QQuick3DCustomMaterialShader *shader)
{
    if (!shader)
        return;

    QQuick3DCustomMaterialRenderPass *that = qobject_cast<QQuick3DCustomMaterialRenderPass *>(list->object);
    that->m_shaders[int(shader->stage)] = shader;
}

QQuick3DCustomMaterialShader *QQuick3DCustomMaterialRenderPass::qmlShaderAt(QQmlListProperty<QQuick3DCustomMaterialShader> *list, int index)
{
    QQuick3DCustomMaterialRenderPass *that = qobject_cast<QQuick3DCustomMaterialRenderPass *>(list->object);
    return that->m_shaders.at(index);
}

int QQuick3DCustomMaterialRenderPass::qmlShaderCount(QQmlListProperty<QQuick3DCustomMaterialShader> *list)
{
    QQuick3DCustomMaterialRenderPass *that = qobject_cast<QQuick3DCustomMaterialRenderPass *>(list->object);
    return that->m_shaders.count();
}

void QQuick3DCustomMaterialRenderPass::qmlShaderClear(QQmlListProperty<QQuick3DCustomMaterialShader> *list)
{
    QQuick3DCustomMaterialRenderPass *that = qobject_cast<QQuick3DCustomMaterialRenderPass *>(list->object);
    auto it = that->m_shaders.begin();
    const auto end = that->m_shaders.end();
    for (;it != end; ++it)
        *it = nullptr;
}

QQmlListProperty<QQuick3DCustomMaterialShader> QQuick3DCustomMaterialRenderPass::shaders()
{
    return QQmlListProperty<QQuick3DCustomMaterialShader>(this,
                                                        nullptr,
                                                        QQuick3DCustomMaterialRenderPass::qmlAppendShader,
                                                        QQuick3DCustomMaterialRenderPass::qmlShaderCount,
                                                        QQuick3DCustomMaterialRenderPass::qmlShaderAt,
                                                        QQuick3DCustomMaterialRenderPass::qmlShaderClear);
}

QT_END_NAMESPACE
