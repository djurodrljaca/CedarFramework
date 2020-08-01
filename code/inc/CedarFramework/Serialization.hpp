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
 * Contains methods used for serialization of a value to JSON
 */

#pragma once

// Cedar Framework includes
#include <CedarFramework/LoggingCategories.hpp>

// Qt includes
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>

// System includes
#include <unordered_map>

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CedarFramework
{

/*!
 * Serializes the value
 *
 * \tparam  T   Value type
 *
 * \param   value   Value to serialize
 *
 * \return  Serialized value or QJsonValue::Undefined in case of an error
 */
template<typename T>
QJsonValue serialize(const T &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const bool &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const int8_t &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const uint8_t &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const int16_t &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const uint16_t &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const int32_t &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const uint32_t &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const int64_t &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const uint64_t &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const float &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const double &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const QChar &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const QString &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const QByteArray &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const QBitArray &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const std::string &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const std::wstring &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const std::u16string &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const std::u32string &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const QDate &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const QTime &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const QDateTime &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const QVariant &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const QUrl &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const QUuid &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const QLocale &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const QRegExp &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const QRegularExpression &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const QSize &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const QSizeF &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const QPoint &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const QPointF &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const QLine &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const QLineF &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const QRect &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const QRectF &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const QStringList &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const QJsonValue &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const QJsonArray &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const QJsonObject &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const QJsonDocument &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const QCborValue &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const QCborArray &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const QCborMap &value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT QJsonValue serialize(const QCborSimpleType &value);

//! \copydoc    CedarFramework::serialize()
template<typename T1, typename T2>
QJsonValue serialize(const QPair<T1, T2> &value);

//! \copydoc    CedarFramework::serialize()
template<typename T1, typename T2>
QJsonValue serialize(const std::pair<T1, T2> &value);

//! \copydoc    CedarFramework::serialize()
template<typename T>
QJsonValue serialize(const QList<T> &value);

//! \copydoc    CedarFramework::serialize()
template<typename T>
QJsonValue serialize(const std::list<T> &value);

//! \copydoc    CedarFramework::serialize()
template<typename T>
QJsonValue serialize(const QVector<T> &value);

//! \copydoc    CedarFramework::serialize()
template<typename T>
QJsonValue serialize(const std::vector<T> &value);

//! \copydoc    CedarFramework::serialize()
template<typename T>
QJsonValue serialize(const QSet<T> &value);

//! \copydoc    CedarFramework::serialize()
template<typename K, typename V>
QJsonValue serialize(const QMap<K, V> &value);

//! \copydoc    CedarFramework::serialize()
template<typename K, typename V>
QJsonValue serialize(const std::map<K, V> &value);

//! \copydoc    CedarFramework::serialize()
template<typename K, typename V>
QJsonValue serialize(const QHash<K, V> &value);

//! \copydoc    CedarFramework::serialize()
template<typename K, typename V>
QJsonValue serialize(const std::unordered_map<K, V> &value);

//! \copydoc    CedarFramework::serialize()
template<typename K, typename V>
QJsonValue serialize(const QMultiMap<K, V> &value);

//! \copydoc    CedarFramework::serialize()
template<typename K, typename V>
QJsonValue serialize(const QMultiHash<K, V> &value);

/*!
 * Helper method for that serializes the key value to a string so that it can be used in a JSON
 * object
 *
 * \tparam  T   Value type
 *
 * \param   value   Key value
 *
 * \param[out]  ok  Output for the flag that shows if the serialization was successful
 *
 * \return  String representation of the key
 *
 * \note    This method accepts only the values that are serialized either to the 'Double' or the
 *          'String' JSON type!
 */
template<typename T>
QString serializeKey(const T &value, bool *ok);

// -------------------------------------------------------------------------------------------------
// Template definitions
// -------------------------------------------------------------------------------------------------

template<typename T1, typename T2>
QJsonValue serialize(const QPair<T1, T2> &value)
{
    return QJsonObject {
        { QStringLiteral("first"), serialize(value.first) },
        { QStringLiteral("second"), serialize(value.second) }
    };
}

// -------------------------------------------------------------------------------------------------

template<typename T1, typename T2>
QJsonValue serialize(const std::pair<T1, T2> &value)
{
    return QJsonObject {
        { QStringLiteral("first"), serialize(value.first) },
        { QStringLiteral("second"), serialize(value.second) }
    };
}

// -------------------------------------------------------------------------------------------------

template<typename T>
QJsonValue serialize(const QList<T> &value)
{
    QJsonArray array;
    int index = 0;

    for (const T &item : value)
    {
        const QJsonValue serializedItem = serialize(item);

        if (serializedItem.isUndefined())
        {
            qCWarning(CedarFramework::LoggingCategory::Serialization)
                    << QStringLiteral("Failed to serialize list item at index:") << index;
            return QJsonValue(QJsonValue::Undefined);
        }

        array.append(serializedItem);
        index++;
    }

    return array;
}

// -------------------------------------------------------------------------------------------------

template<typename T>
QJsonValue serialize(const std::list<T> &value)
{
    QJsonArray array;
    int index = 0;

    for (const T &item : value)
    {
        const QJsonValue serializedItem = serialize(item);

        if (serializedItem.isUndefined())
        {
            qCWarning(CedarFramework::LoggingCategory::Serialization)
                    << QStringLiteral("Failed to serialize list item at index:") << index;
            return QJsonValue(QJsonValue::Undefined);
        }

        array.append(serializedItem);
        index++;
    }

    return array;
}

// -------------------------------------------------------------------------------------------------

template<typename T>
QJsonValue serialize(const QVector<T> &value)
{
    QJsonArray array;
    int index = 0;

    for (const T &item : value)
    {
        const QJsonValue serializedItem = serialize(item);

        if (serializedItem.isUndefined())
        {
            qCWarning(CedarFramework::LoggingCategory::Serialization)
                    << QStringLiteral("Failed to serialize vector item at index:") << index;
            return QJsonValue(QJsonValue::Undefined);
        }

        array.append(serializedItem);
        index++;
    }

    return array;
}

// -------------------------------------------------------------------------------------------------

template<typename T>
QJsonValue serialize(const std::vector<T> &value)
{
    QJsonArray array;
    int index = 0;

    for (const T &item : value)
    {
        const QJsonValue serializedItem = serialize(item);

        if (serializedItem.isUndefined())
        {
            qCWarning(CedarFramework::LoggingCategory::Serialization)
                    << QStringLiteral("Failed to serialize vector item at index:") << index;
            return QJsonValue(QJsonValue::Undefined);
        }

        array.append(serializedItem);
        index++;
    }

    return array;
}

// -------------------------------------------------------------------------------------------------

template<typename T>
QJsonValue serialize(const QSet<T> &value)
{
    QJsonArray array;

    for (const T &item : value)
    {
        const QJsonValue serializedItem = serialize(item);

        if (serializedItem.isUndefined())
        {
            qCWarning(CedarFramework::LoggingCategory::Serialization)
                    << QStringLiteral("Failed to serialize set item");
            return QJsonValue(QJsonValue::Undefined);
        }

        array.append(serializedItem);
    }

    return array;
}

// -------------------------------------------------------------------------------------------------

template<typename K, typename V>
QJsonValue serialize(const QMap<K, V> &value)
{
    QJsonObject object;

    for (auto it = value.cbegin(); it != value.cend(); it++)
    {
        // Serialize key
        bool ok = false;
        const QString serializedKey = serializeKey(it.key(), &ok);

        if (!ok)
        {
            qCWarning(CedarFramework::LoggingCategory::Serialization)
                    << QStringLiteral("Failed to serialize the item's key");
            return QJsonValue(QJsonValue::Undefined);
        }

        if (object.contains(serializedKey))
        {
            qCWarning(CedarFramework::LoggingCategory::Serialization)
                    << QStringLiteral("Duplicate key:") << serializedKey;
            return QJsonValue(QJsonValue::Undefined);
        }

        // Serialize value
        const QJsonValue serializedValue = serialize(it.value());

        if (serializedValue.isUndefined())
        {
            qCWarning(CedarFramework::LoggingCategory::Serialization)
                    << QStringLiteral("Failed to serialize the item's value with key:")
                    << serializedKey;
            return QJsonValue(QJsonValue::Undefined);
        }

        object.insert(serializedKey, serializedValue);
    }

    return object;
}

// -------------------------------------------------------------------------------------------------

template<typename K, typename V>
QJsonValue serialize(const std::map<K, V> &value)
{
    QJsonObject object;

    for (const auto &it : value)
    {
        // Serialize key
        bool ok = false;
        const QString serializedKey = serializeKey(it.first, &ok);

        if (!ok)
        {
            qCWarning(CedarFramework::LoggingCategory::Serialization)
                    << QStringLiteral("Failed to serialize the item's key");
            return QJsonValue(QJsonValue::Undefined);
        }

        if (object.contains(serializedKey))
        {
            qCWarning(CedarFramework::LoggingCategory::Serialization)
                    << QStringLiteral("Duplicate key:") << serializedKey;
            return QJsonValue(QJsonValue::Undefined);
        }

        // Serialize value
        const QJsonValue serializedValue = serialize(it.second);

        if (serializedValue.isUndefined())
        {
            qCWarning(CedarFramework::LoggingCategory::Serialization)
                    << QStringLiteral("Failed to serialize the item's value with key:")
                    << serializedKey;
            return QJsonValue(QJsonValue::Undefined);
        }

        object.insert(serializedKey, serializedValue);
    }

    return object;
}

// -------------------------------------------------------------------------------------------------

template<typename K, typename V>
QJsonValue serialize(const QHash<K, V> &value)
{
    QJsonObject object;

    for (auto it = value.cbegin(); it != value.cend(); it++)
    {
        // Serialize key
        bool ok = false;
        const QString serializedKey = serializeKey(it.key(), &ok);

        if (!ok)
        {
            qCWarning(CedarFramework::LoggingCategory::Serialization)
                    << QStringLiteral("Failed to serialize the item's key");
            return QJsonValue(QJsonValue::Undefined);
        }

        if (object.contains(serializedKey))
        {
            qCWarning(CedarFramework::LoggingCategory::Serialization)
                    << QStringLiteral("Duplicate key:") << serializedKey;
            return QJsonValue(QJsonValue::Undefined);
        }

        // Serialize value
        const QJsonValue serializedValue = serialize(it.value());

        if (serializedValue.isUndefined())
        {
            qCWarning(CedarFramework::LoggingCategory::Serialization)
                    << QStringLiteral("Failed to serialize the item's value with key:")
                    << serializedKey;
            return QJsonValue(QJsonValue::Undefined);
        }

        object.insert(serializedKey, serializedValue);
    }

    return object;
}

// -------------------------------------------------------------------------------------------------

template<typename K, typename V>
QJsonValue serialize(const std::unordered_map<K, V> &value)
{
    QJsonObject object;

    for (const auto &it : value)
    {
        // Serialize key
        bool ok = false;
        const QString serializedKey = serializeKey(it.first, &ok);

        if (!ok)
        {
            qCWarning(CedarFramework::LoggingCategory::Serialization)
                    << QStringLiteral("Failed to serialize the item's key");
            return QJsonValue(QJsonValue::Undefined);
        }

        if (object.contains(serializedKey))
        {
            qCWarning(CedarFramework::LoggingCategory::Serialization)
                    << QStringLiteral("Duplicate key:") << serializedKey;
            return QJsonValue(QJsonValue::Undefined);
        }

        // Serialize value
        const QJsonValue serializedValue = serialize(it.second);

        if (serializedValue.isUndefined())
        {
            qCWarning(CedarFramework::LoggingCategory::Serialization)
                    << QStringLiteral("Failed to serialize the item's value with key:")
                    << serializedKey;
            return QJsonValue(QJsonValue::Undefined);
        }

        object.insert(serializedKey, serializedValue);
    }

    return object;
}

// -------------------------------------------------------------------------------------------------

template<typename K, typename V>
QJsonValue serialize(const QMultiMap<K, V> &value)
{
    QJsonObject object;

    for (const auto &key : value.uniqueKeys())
    {
        // Serialize key
        bool ok = false;
        const QString serializedKey = serializeKey(key, &ok);

        if (!ok)
        {
            qCWarning(CedarFramework::LoggingCategory::Serialization)
                    << QStringLiteral("Failed to serialize the item's key");
            return QJsonValue(QJsonValue::Undefined);
        }

        if (object.contains(serializedKey))
        {
            qCWarning(CedarFramework::LoggingCategory::Serialization)
                    << QStringLiteral("Duplicate key:") << serializedKey;
            return QJsonValue(QJsonValue::Undefined);
        }

        // Serialize value
        const QJsonValue serializedValue = serialize(value.values(key));

        if (serializedValue.isUndefined())
        {
            qCWarning(CedarFramework::LoggingCategory::Serialization)
                    << QStringLiteral("Failed to serialize the item's value with key:")
                    << serializedKey;
            return QJsonValue(QJsonValue::Undefined);
        }

        object.insert(serializedKey, serializedValue);
    }

    return object;
}

// -------------------------------------------------------------------------------------------------

template<typename K, typename V>
QJsonValue serialize(const QMultiHash<K, V> &value)
{
    QJsonObject object;

    for (const auto &key : value.uniqueKeys())
    {
        // Serialize key
        bool ok = false;
        const QString serializedKey = serializeKey(key, &ok);

        if (!ok)
        {
            qCWarning(CedarFramework::LoggingCategory::Serialization)
                    << QStringLiteral("Failed to serialize the item's key");
            return QJsonValue(QJsonValue::Undefined);
        }

        if (object.contains(serializedKey))
        {
            qCWarning(CedarFramework::LoggingCategory::Serialization)
                    << QStringLiteral("Duplicate key:") << serializedKey;
            return QJsonValue(QJsonValue::Undefined);
        }

        // Serialize value
        const QJsonValue serializedValue = serialize(value.values(key));

        if (serializedValue.isUndefined())
        {
            qCWarning(CedarFramework::LoggingCategory::Serialization)
                    << QStringLiteral("Failed to serialize the item's value with key:")
                    << serializedKey;
            return QJsonValue(QJsonValue::Undefined);
        }

        object.insert(serializedKey, serializedValue);
    }

    return object;
}

// -------------------------------------------------------------------------------------------------

template<typename T>
QString serializeKey(const T &value, bool *ok)
{
    Q_ASSERT(ok != nullptr);

    const QJsonValue serializedKey = serialize(value);

    switch (serializedKey.type())
    {
        case QJsonValue::Double:
        {
            *ok = true;
            return QString::number(serializedKey.toDouble());
        }

        case QJsonValue::String:
        {
            *ok = true;
            return serializedKey.toString();
        }

        default:
        {
            *ok = false;
            qCWarning(CedarFramework::LoggingCategory::Serialization)
                    << QStringLiteral("Failed to convert the serialized key value to a string");
            return {};
        }
    }
}

} // namespace CedarFramework
