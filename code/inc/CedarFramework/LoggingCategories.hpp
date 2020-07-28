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
 * Contains logging categories
 */

#ifndef CEDARFRAMEWORK_LOGGINGCATEGORIES_HPP
#define CEDARFRAMEWORK_LOGGINGCATEGORIES_HPP

// Cedar Framework includes
#include <CedarFramework/CedarFrameworkExport.hpp>

// Qt includes
#include <QtCore/QLoggingCategory>

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CedarFramework
{

namespace LoggingCategory
{

//! Logging category for serialization
CEDARFRAMEWORK_EXPORT extern const QLoggingCategory Serialization;

} // namespace LoggingCategory

} // namespace CedarFramework

#endif // CEDARFRAMEWORK_LOGGINGCATEGORIES_HPP
