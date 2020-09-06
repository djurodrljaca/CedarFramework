/* This file is part of Cedar Framework.
 *
 * Cedar Framework is free software: you can redistribute it and/or modify it under the terms
 * of the GNU Lesser General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * Cedar Framework is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with Cedar
 * Framework. If not, see <http://www.gnu.org/licenses/>.
 */

/*!
 * \file
 *
 * Contains methods used for querying JSON structures
 */

// Own header
#include <CedarFramework/Query.hpp>

// Cedar Framework includes

// Qt includes
#include <QtCore/QJsonValue>

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CedarFramework
{

bool hasNode(const QJsonValue &data, const int index)
{
    return (!getNode(data, index).isUndefined());
}

// -------------------------------------------------------------------------------------------------

bool hasNode(const QJsonValue &data, const QString &name)
{
    return (!getNode(data, name).isUndefined());
}

// -------------------------------------------------------------------------------------------------

bool hasNode(const QJsonValue &data, const QVariantList &nodePath)
{
    return (!getNode(data, nodePath).isUndefined());
}

// -------------------------------------------------------------------------------------------------

bool hasNode(const QJsonValue &data, const QStringList &nodePath)
{
    return (!getNode(data, nodePath).isUndefined());
}

// -------------------------------------------------------------------------------------------------

QJsonValue getNode(const QJsonValue &data, const int index)
{
    // Note: if sub-node is not found an Undefined value is returned
    return data[index];
}

// -------------------------------------------------------------------------------------------------

QJsonValue getNode(const QJsonValue &data, const QString &name)
{
    // Note: if sub-node is not found an Undefined value is returned
    return data[name];
}

// -------------------------------------------------------------------------------------------------

QJsonValue getNode(const QJsonValue &data, const QVariantList &nodePath)
{
    QJsonValue node = data;

    for (const QVariant &nodePathItem : nodePath)
    {
        switch (node.type())
        {
            case QJsonValue::Array:
            {
                bool ok = false;
                const int index = nodePathItem.toInt(&ok);

                if (!ok)
                {
                    // Not an index
                    return QJsonValue::Undefined;
                }

                // Note: if sub-node is not found an Undefined value is returned
                node = node[index];
                break;
            }

            case QJsonValue::Object:
            {
                if (!nodePathItem.canConvert<QString>())
                {
                    // Not a name of a member
                    return QJsonValue::Undefined;
                }

                // Note: if sub-node is not found an Undefined value is returned
                node = node[nodePathItem.toString()];
                break;
            }

            default:
            {
                // Only Array and Object types have sub-nodes!
                return QJsonValue::Undefined;
            }
        }
    }

    return node;
}

// -------------------------------------------------------------------------------------------------

QJsonValue getNode(const QJsonValue &data, const QStringList &nodePath)
{
    QJsonValue node = data;

    for (const QString &nodePathItem : nodePath)
    {
        switch (node.type())
        {
            case QJsonValue::Array:
            {
                bool ok = false;
                const int index = nodePathItem.toInt(&ok);

                if (!ok)
                {
                    // Not an index
                    return QJsonValue::Undefined;
                }

                // Note: if sub-node is not found an Undefined value is returned
                node = node[index];
                break;
            }

            case QJsonValue::Object:
            {
                // Note: if sub-node is not found an Undefined value is returned
                node = node[nodePathItem];
                break;
            }

            default:
            {
                // Only Array and Object types have sub-nodes!
                return QJsonValue::Undefined;
            }
        }
    }

    return node;
}

} // namespace CedarFramework
