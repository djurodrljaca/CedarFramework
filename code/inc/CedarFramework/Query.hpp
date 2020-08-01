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

#pragma once

// Cedar Framework includes
#include <CedarFramework/LoggingCategories.hpp>

// Qt includes

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CedarFramework
{

/*!
 * Checks if the data contains a sub-node at the specified index
 *
 * \param data  Data to query
 * \param index Sub-node index
 *
 * \retval  true    Node was found
 * \retval  false   Node was not
 */
bool hasNode(const QJsonValue &data, const int index);

/*!
 * Checks if the data contains a sub-node with the specified name
 *
 * \param data  Data to query
 * \param name  Sub-node name
 *
 * \retval  true    Node was found
 * \retval  false   Node was not
 */
bool hasNode(const QJsonValue &data, const QString &name);

/*!
 * Checks if the data contains a sub-node at the specified path
 *
 * \param data      Data to query
 * \param nodePath  Path to the node (list of indexes and/or member names)
 *
 * \retval  true    Node was found
 * \retval  false   Node was not
 */
bool hasNode(const QJsonValue &data, const QVariantList &nodePath);

/*!
 * Gets the sub-node at the specified index
 *
 * \param data  Data to query
 * \param index Sub-node index
 *
 * \return  Node at the specified index or an Undefined value if the node was not found
 */
QJsonValue getNode(const QJsonValue &data, const int index);

/*!
 * Gets the sub-node with the specified name
 *
 * \param data  Data to query
 * \param name  Sub-node name
 *
 * \return  Node with the specified name or an Undefined value if the node was not found
 */
QJsonValue getNode(const QJsonValue &data, const QString &name);

/*!
 * Gets the sub-node at the specified path
 *
 * \param data      Data to query
 * \param nodePath  Path to the node (list of indexes and/or member names)
 *
 * \return  Node at the specified path or an Undefined value if the node was not found
 */
QJsonValue getNode(const QJsonValue &data, const QVariantList &nodePath);

} // namespace CedarFramework
