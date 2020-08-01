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
 * Contains methods used for deserialization of JSON to a value
 */

#pragma once

// Cedar Framework includes
#include <CedarFramework/Query.hpp>

// Qt includes
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>
#include <QtCore/QMap>

// System includes
#include <unordered_map>

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CedarFramework
{

/*!
 * Deserializes the value
 *
 * \tparam  T   Value type
 *
 * \param   json    JSON value to deserialize
 *
 * \param[out]  value   Output for the deserialized value
 *
 * \retval  true    Success
 * \retval  false   Failure
 */
template<typename T>
bool deserialize(const QJsonValue &json, T *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, bool *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, int8_t *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, uint8_t *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, int16_t *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, uint16_t *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, int32_t *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, uint32_t *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, int64_t *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, uint64_t *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, float *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, double *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, QChar *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, QString *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, QByteArray *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, QBitArray *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, std::string *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, std::wstring *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, std::u16string *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, std::u32string *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, QDate *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, QTime *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, QDateTime *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, QVariant *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, QUrl *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, QUuid *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, QLocale *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, QRegExp *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, QRegularExpression *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, QSize *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, QSizeF *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, QPoint *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, QPointF *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, QLine *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, QLineF *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, QRect *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, QRectF *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, QStringList *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, QJsonValue *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, QJsonArray *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, QJsonObject *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, QJsonDocument *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, QCborValue *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, QCborArray *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, QCborMap *value);

//! \copydoc    CedarFramework::serialize()
template<>
CEDARFRAMEWORK_EXPORT bool deserialize(const QJsonValue &json, QCborSimpleType *value);

//! \copydoc    CedarFramework::serialize()
template<typename T1, typename T2>
bool deserialize(const QJsonValue &json, QPair<T1, T2> *value);

//! \copydoc    CedarFramework::serialize()
template<typename T1, typename T2>
bool deserialize(const QJsonValue &json, std::pair<T1, T2> *value);

//! \copydoc    CedarFramework::serialize()
template<typename T>
bool deserialize(const QJsonValue &json, QList<T> *value);

//! \copydoc    CedarFramework::serialize()
template<typename T>
bool deserialize(const QJsonValue &json, std::list<T> *value);

//! \copydoc    CedarFramework::serialize()
template<typename T>
bool deserialize(const QJsonValue &json, QVector<T> *value);

//! \copydoc    CedarFramework::serialize()
template<typename T>
bool deserialize(const QJsonValue &json, std::vector<T> *value);

//! \copydoc    CedarFramework::serialize()
template<typename T>
bool deserialize(const QJsonValue &json, QSet<T> *value);

//! \copydoc    CedarFramework::serialize()
template<typename K, typename V>
bool deserialize(const QJsonValue &json, QMap<K, V> *value);

//! \copydoc    CedarFramework::serialize()
template<typename K, typename V>
bool deserialize(const QJsonValue &json, std::map<K, V> *value);

//! \copydoc    CedarFramework::serialize()
template<typename K, typename V>
bool deserialize(const QJsonValue &json, QHash<K, V> *value);

//! \copydoc    CedarFramework::serialize()
template<typename K, typename V>
bool deserialize(const QJsonValue &json, std::unordered_map<K, V> *value);

//! \copydoc    CedarFramework::serialize()
template<typename K, typename V>
bool deserialize(const QJsonValue &json, QMultiMap<K, V> *value);

//! \copydoc    CedarFramework::serialize()
template<typename K, typename V>
bool deserialize(const QJsonValue &json, QMultiHash<K, V> *value);

/*!
 * Helper method for that deserializes the key value from a key (string) in a JSON object
 *
 * \tparam  T   Value type
 *
 * \param   value   Key value
 *
 * \param[out]  key     Output for the deserialized key
 *
 * \retval  true    Success
 * \retval  false   Failure
 */
template<typename T>
bool deserializeKey(const QString &value, T *key);

/*!
 * Deserializes the sub-node at the specified index
 *
 * \tparam  T   Value type
 *
 * \param data  Data to query
 * \param index Sub-node index
 *
 * \param[out]  value   Output for the deserialized value
 *
 * \retval  true    Success
 * \retval  true    Failure
 */
template<typename T>
bool deserializeNode(const QJsonValue &data, const int index, T *value);

/*!
 * Deserializes the sub-node with the specified name
 *
 * \tparam  T   Value type
 *
 * \param data  Data to query
 * \param name  Sub-node name
 *
 * \param[out]  value   Output for the deserialized value
 *
 * \retval  true    Success
 * \retval  true    Failure
 */
template<typename T>
bool deserializeNode(const QJsonValue &data, const int index, T *value);

/*!
 * Deserializes the sub-node at the specified path
 *
 * \tparam  T   Value type
 *
 * \param data      Data to query
 * \param nodePath  Path to the node (list of indexes and/or member names)
 *
 * \param[out]  value   Output for the deserialized value
 *
 * \retval  true    Success
 * \retval  true    Failure
 */
template<typename T>
bool deserializeNode(const QJsonValue &data, const QVariantList &nodePath, T *value);

// -------------------------------------------------------------------------------------------------
// Template definitions
// -------------------------------------------------------------------------------------------------

template<typename T1, typename T2>
bool deserialize(const QJsonValue &json, QPair<T1, T2> *value)
{
    Q_ASSERT(value != nullptr);

    // Get the JSON Object representation
    if (!json.isObject())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("JSON value is not an Object");
        return false;
    }

    const auto jsonObject = json.toObject();

    if (jsonObject.size() != 2)
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("A pair needs to have exactly two members but this one has:")
                << jsonObject.size();
        return false;
    }

    // Deserialize members
    if (!deserialize(jsonObject.value(QStringLiteral("first")), &value->first))
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("Failed to deserialize the member 'first' of a pair item");
        return false;
    }

    if (!deserialize(jsonObject.value(QStringLiteral("second")), &value->second))
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("Failed to deserialize the member 'second' of a pair item");
        return false;
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename T1, typename T2>
bool deserialize(const QJsonValue &json, std::pair<T1, T2> *value)
{
    Q_ASSERT(value != nullptr);

    // Get the JSON Object representation
    if (!json.isObject())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("JSON value is not an Object");
        return false;
    }

    const auto jsonObject = json.toObject();

    if (jsonObject.size() != 2)
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("A pair needs to have exactly two members but this one has:")
                << jsonObject.size();
        return false;
    }

    // Deserialize members
    if (!deserialize(jsonObject.value(QStringLiteral("first")), &value->first))
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("Failed to deserialize the member 'first' of a pair item");
        return false;
    }

    if (!deserialize(jsonObject.value(QStringLiteral("second")), &value->second))
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("Failed to deserialize the member 'second' of a pair item");
        return false;
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename T>
bool deserialize(const QJsonValue &json, QList<T> *value)
{
    Q_ASSERT(value != nullptr);

    // Get the JSON Array representation
    if (!json.isArray())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("JSON value is not an Array");
        return false;
    }

    const auto jsonArray = json.toArray();

    // Deserialize elements
    value->clear();
    int index = 0;

    for (const auto &item : jsonArray)
    {
        T deserializedItem;

        if (!deserialize(item, &deserializedItem))
        {
            qCWarning(CedarFramework::LoggingCategory::Deserialization)
                    << QStringLiteral("Failed to deserialize the list element at index:") << index;
            return false;
        }

        value->append(deserializedItem);
        index++;
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename T>
bool deserialize(const QJsonValue &json, std::list<T> *value)
{
    Q_ASSERT(value != nullptr);

    // Get the JSON Array representation
    if (!json.isArray())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("JSON value is not an Array");
        return false;
    }

    const auto jsonArray = json.toArray();

    // Deserialize elements
    value->clear();
    int index = 0;

    for (const auto &item : jsonArray)
    {
        T deserializedItem;

        if (!deserialize(item, &deserializedItem))
        {
            qCWarning(CedarFramework::LoggingCategory::Deserialization)
                    << QStringLiteral("Failed to deserialize the list element at index:") << index;
            return false;
        }

        value->push_back(std::move(deserializedItem));
        index++;
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename T>
bool deserialize(const QJsonValue &json, QVector<T> *value)
{
    Q_ASSERT(value != nullptr);

    // Get the JSON Array representation
    if (!json.isArray())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("JSON value is not an Array");
        return false;
    }

    const auto jsonArray = json.toArray();

    // Deserialize elements
    value->clear();
    int index = 0;

    for (const auto &item : jsonArray)
    {
        T deserializedItem;

        if (!deserialize(item, &deserializedItem))
        {
            qCWarning(CedarFramework::LoggingCategory::Deserialization)
                    << QStringLiteral("Failed to deserialize the vector element at index:")
                    << index;
            return false;
        }

        value->append(deserializedItem);
        index++;
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename T>
bool deserialize(const QJsonValue &json, std::vector<T> *value)
{
    Q_ASSERT(value != nullptr);

    // Get the JSON Array representation
    if (!json.isArray())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("JSON value is not an Array");
        return false;
    }

    const auto jsonArray = json.toArray();

    // Deserialize elements
    value->clear();
    int index = 0;

    for (const auto &item : jsonArray)
    {
        T deserializedItem;

        if (!deserialize(item, &deserializedItem))
        {
            qCWarning(CedarFramework::LoggingCategory::Deserialization)
                    << QStringLiteral("Failed to deserialize the vector element at index:")
                    << index;
            return false;
        }

        value->push_back(std::move(deserializedItem));
        index++;
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename T>
bool deserialize(const QJsonValue &json, QSet<T> *value)
{
    Q_ASSERT(value != nullptr);

    // Get the JSON Array representation
    if (!json.isArray())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("JSON value is not an Array");
        return false;
    }

    const auto jsonArray = json.toArray();

    // Deserialize elements
    value->clear();

    for (const auto &item : jsonArray)
    {
        T deserializedItem;

        if (!deserialize(item, &deserializedItem))
        {
            qCWarning(CedarFramework::LoggingCategory::Deserialization)
                    << QStringLiteral("Failed to deserialize the set element");
            return false;
        }

        if (value->contains(deserializedItem))
        {
            qCWarning(CedarFramework::LoggingCategory::Deserialization)
                    << QStringLiteral("Duplicate set element");
            return false;
        }

        value->insert(deserializedItem);
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename K, typename V>
bool deserialize(const QJsonValue &json, QMap<K, V> *value)
{
    Q_ASSERT(value != nullptr);

    // Get the JSON Object representation
    if (!json.isObject())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("JSON value is not an Object");
        return false;
    }

    const auto jsonObject = json.toObject();

    // Deserialize members
    value->clear();

    for (auto it = jsonObject.begin(); it != jsonObject.end(); it++)
    {
        // Deserialize key
        K deserializedKey;

        if (!deserializeKey(it.key(), &deserializedKey))
        {
            qCWarning(CedarFramework::LoggingCategory::Deserialization)
                    << QStringLiteral("Failed to deserialize the key in a map");
            return false;
        }

        // Deserialize value
        V deserializedValue;

        if (deserialize(it.value(), &deserializedValue))
        {
            qCWarning(CedarFramework::LoggingCategory::Deserialization)
                    << QStringLiteral("Failed to deserialize the map item's value with key:")
                    << it.key();
            return false;
        }

        value->insert(deserializedKey, deserializedValue);
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename K, typename V>
bool deserialize(const QJsonValue &json, std::map<K, V> *value)
{
    Q_ASSERT(value != nullptr);

    // Get the JSON Object representation
    if (!json.isObject())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("JSON value is not an Object");
        return false;
    }

    const auto jsonObject = json.toObject();

    // Deserialize members
    value->clear();

    for (auto it = jsonObject.begin(); it != jsonObject.end(); it++)
    {
        // Deserialize key
        K deserializedKey;

        if (!deserializeKey(it.key(), &deserializedKey))
        {
            qCWarning(CedarFramework::LoggingCategory::Deserialization)
                    << QStringLiteral("Failed to deserialize the key in a map");
            return false;
        }

        // Deserialize value
        V deserializedValue;

        if (deserialize(it.value(), &deserializedValue))
        {
            qCWarning(CedarFramework::LoggingCategory::Deserialization)
                    << QStringLiteral("Failed to deserialize the map item's value with key:")
                    << it.key();
            return false;
        }

        value->emplace(deserializedKey, deserializedValue);
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename K, typename V>
bool deserialize(const QJsonValue &json, QHash<K, V> *value)
{
    Q_ASSERT(value != nullptr);

    // Get the JSON Object representation
    if (!json.isObject())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("JSON value is not an Object");
        return false;
    }

    const auto jsonObject = json.toObject();

    // Deserialize members
    value->clear();

    for (auto it = jsonObject.begin(); it != jsonObject.end(); it++)
    {
        // Deserialize key
        K deserializedKey;

        if (!deserializeKey(it.key(), &deserializedKey))
        {
            qCWarning(CedarFramework::LoggingCategory::Deserialization)
                    << QStringLiteral("Failed to deserialize the key in a hash");
            return false;
        }

        // Deserialize value
        V deserializedValue;

        if (deserialize(it.value(), &deserializedValue))
        {
            qCWarning(CedarFramework::LoggingCategory::Deserialization)
                    << QStringLiteral("Failed to deserialize the hash item's value with key:")
                    << it.key();
            return false;
        }

        value->insert(deserializedKey, deserializedValue);
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename K, typename V>
bool deserialize(const QJsonValue &json, std::unordered_map<K, V> *value)
{
    Q_ASSERT(value != nullptr);

    // Get the JSON Object representation
    if (!json.isObject())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("JSON value is not an Object");
        return false;
    }

    const auto jsonObject = json.toObject();

    // Deserialize members
    value->clear();

    for (auto it = jsonObject.begin(); it != jsonObject.end(); it++)
    {
        // Deserialize key
        K deserializedKey;

        if (!deserializeKey(it.key(), &deserializedKey))
        {
            qCWarning(CedarFramework::LoggingCategory::Deserialization)
                    << QStringLiteral("Failed to deserialize the key in an unordered map");
            return false;
        }

        // Deserialize value
        V deserializedValue;

        if (deserialize(it.value(), &deserializedValue))
        {
            qCWarning(CedarFramework::LoggingCategory::Deserialization)
                    << QStringLiteral("Failed to deserialize the unordered map item's value with "
                                      "key:") << it.key();
            return false;
        }

        value->emplace(deserializedKey, deserializedValue);
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename K, typename V>
bool deserialize(const QJsonValue &json, QMultiMap<K, V> *value)
{
    Q_ASSERT(value != nullptr);

    // Get the JSON Object representation
    if (!json.isObject())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("JSON value is not an Object");
        return false;
    }

    const auto jsonObject = json.toObject();

    // Deserialize members
    value->clear();

    for (auto it = jsonObject.begin(); it != jsonObject.end(); it++)
    {
        // Deserialize key
        K deserializedKey;

        if (!deserializeKey(it.key(), &deserializedKey))
        {
            qCWarning(CedarFramework::LoggingCategory::Deserialization)
                    << QStringLiteral("Failed to deserialize the key in a multi map");
            return false;
        }

        // Deserialize values
        QList<V> deserializedValues;

        if (deserialize(it.value(), &deserializedValues))
        {
            qCWarning(CedarFramework::LoggingCategory::Deserialization)
                    << QStringLiteral("Failed to deserialize the multi map item's value with key:")
                    << it.key();
            return false;
        }

        for (const auto &item : deserializedValues)
        {
            value->insert(deserializedKey, item);
        }
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename K, typename V>
bool deserialize(const QJsonValue &json, QMultiHash<K, V> *value)
{
    Q_ASSERT(value != nullptr);

    // Get the JSON Object representation
    if (!json.isObject())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("JSON value is not an Object");
        return false;
    }

    const auto jsonObject = json.toObject();

    // Deserialize members
    value->clear();

    for (auto it = jsonObject.begin(); it != jsonObject.end(); it++)
    {
        // Deserialize key
        K deserializedKey;

        if (!deserializeKey(it.key(), &deserializedKey))
        {
            qCWarning(CedarFramework::LoggingCategory::Deserialization)
                    << QStringLiteral("Failed to deserialize the key in a multi map");
            return false;
        }

        // Deserialize values
        QList<V> deserializedValues;

        if (deserialize(it.value(), &deserializedValues))
        {
            qCWarning(CedarFramework::LoggingCategory::Deserialization)
                    << QStringLiteral("Failed to deserialize the multi hash item's value with key:")
                    << it.key();
            return false;
        }

        for (const auto &item : deserializedValues)
        {
            value->insert(deserializedKey, item);
        }
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename T>
bool deserializeKey(const QString &value, T *key)
{
    Q_ASSERT(value != nullptr);

    return deserialize(QJsonValue(value), key);
}

// -------------------------------------------------------------------------------------------------

template<typename T>
bool deserializeNode(const QJsonValue &data, const int index, T *value)
{
    const QJsonValue node = getNode(data, index);

    if (node.isUndefined())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("Failed to find the specified node");
        return false;
    }

    return deserialize(node, value);
}

// -------------------------------------------------------------------------------------------------

template<typename T>
bool deserializeNode(const QJsonValue &data, const QString &name, T *value)
{
    const QJsonValue node = getNode(data, name);

    if (node.isUndefined())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("Failed to find the specified node");
        return false;
    }

    return deserialize(node, value);
}

// -------------------------------------------------------------------------------------------------

template<typename T>
bool deserializeNode(const QJsonValue &data, const QVariantList &nodePath, T *value)
{
    const QJsonValue node = getNode(data, nodePath);

    if (node.isUndefined())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("Failed to find the specified node");
        return false;
    }

    return deserialize(node, value);
}

} // namespace CedarFramework
