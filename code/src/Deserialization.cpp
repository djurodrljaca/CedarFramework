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

// Own header
#include <CedarFramework/Deserialization.hpp>

// Cedar Framework includes

// Qt includes
#include <QtCore/QBitArray>
#include <QtCore/QCborArray>
#include <QtCore/QCborMap>
#include <QtCore/QDateTime>
#include <QtCore/QJsonDocument>
#include <QtCore/QLine>
#include <QtCore/QLineF>
#include <QtCore/QPoint>
#include <QtCore/QPointF>
#include <QtCore/QRect>
#include <QtCore/QRectF>
#include <QtCore/QSize>
#include <QtCore/QSizeF>
#include <QtCore/QUrl>
#include <QtCore/QUuid>

// System includes
#include <cmath>

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CedarFramework
{

// -------------------------------------------------------------------------------------------------
// Helper methods
// -------------------------------------------------------------------------------------------------

namespace Internal
{

template<typename T>
using IsMax64BitInteger = std::enable_if_t<std::is_integral<T>::value && (sizeof(T) <= 8), bool>;

template<typename T>
using IsMax64BitFloatingPoint = std::enable_if_t<
std::is_floating_point<T>::value && (sizeof(T) <= 8),
bool
>;

// -------------------------------------------------------------------------------------------------

template<typename T_OUT, IsMax64BitInteger<T_OUT> = true>
bool convertIntegerValue(const qint64 inputValue, T_OUT *outputValue)
{
    constexpr auto lowwerLimit = std::numeric_limits<T_OUT>::min();
    constexpr auto upperLimit = std::numeric_limits<T_OUT>::max();

    // Check if output value is also signed
    if (std::is_signed<T_OUT>::value)
    {
        // Output value is also signed, just check the valid range
        if ((inputValue < static_cast<qint64>(lowwerLimit)) ||
            (inputValue > static_cast<qint64>(upperLimit)))
        {
            qCWarning(CedarFramework::LoggingCategory::Deserialization)
                    << QString("Value [%1] is out of range for the its data type "
                               "(min: [%2], max: [%3])!")
                       .arg(inputValue)
                       .arg(lowwerLimit)
                       .arg(upperLimit);
            return false;
        }
    }
    else
    {
        // Output is unsigned

        // A negative value cannot be stored in the output or a positive value greater than the
        // max value that can be stored in the output
        if ((inputValue < 0LL) ||
            (static_cast<quint64>(inputValue) > static_cast<quint64>(upperLimit)))
        {
            qCWarning(CedarFramework::LoggingCategory::Deserialization)
                    << QString("Value [%1] is out of range for the its data type "
                               "(min: [%2], max: [%3])!")
                       .arg(inputValue)
                       .arg(lowwerLimit)
                       .arg(upperLimit);
            return false;
        }
    }

    *outputValue = static_cast<T_OUT>(inputValue);
    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename T_OUT, IsMax64BitInteger<T_OUT> = true>
bool convertIntegerValue(const quint64 inputValue, T_OUT *outputValue)
{
    constexpr auto lowwerLimit = std::numeric_limits<T_OUT>::min();
    constexpr auto upperLimit = std::numeric_limits<T_OUT>::max();

    // Make sure that the input value can be stored in the output
    if (inputValue > static_cast<quint64>(upperLimit))
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QString("Value [%1] is out of range for the its data type "
                           "(min: [%2], max: [%3])!")
                   .arg(inputValue)
                   .arg(lowwerLimit)
                   .arg(upperLimit);
        return false;
    }

    *outputValue = static_cast<T_OUT>(inputValue);
    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename T_OUT>
bool convertIntegerValue(const double inputValue, T_OUT *outputValue)
{
    constexpr auto lowwerLimit = static_cast<double>(std::numeric_limits<T_OUT>::min());
    constexpr auto upperLimit  = static_cast<double>(std::numeric_limits<T_OUT>::max());

    // Make sure that the input value can be stored in the output
    if ((inputValue < lowwerLimit) ||
        (inputValue > upperLimit))
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QString("Value [%1] is out of range for the its data type "
                           "(min: [%2], max: [%3])!")
                   .arg(inputValue, 0, 'f')
                   .arg(lowwerLimit, 0, 'f')
                   .arg(upperLimit, 0, 'f');
        return false;
    }

    *outputValue = static_cast<T_OUT>(std::round(inputValue));
    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename T_OUT>
bool convertIntegerValue(const QString inputValue, T_OUT *outputValue)
{
    // Through a signed integer
    {
        bool ok = false;
        const qint64 integerValue = inputValue.toLongLong(&ok);

        if (ok)
        {
            return convertIntegerValue(integerValue, outputValue);
        }
    }

    // Through an unsigned integer
    {
        bool ok = false;
        const quint64 integerValue = inputValue.toULongLong(&ok);

        if (ok)
        {
            return convertIntegerValue(integerValue, outputValue);
        }
    }

    // Through a floating-point
    {
        bool ok = false;
        const double doubleValue = inputValue.toDouble(&ok);

        if (ok)
        {
            return convertIntegerValue(doubleValue, outputValue);
        }
    }

    qCWarning(CedarFramework::LoggingCategory::Deserialization)
            << QString("Value [%1] is not a valid integer!").arg(inputValue);
    return false;
}

// -------------------------------------------------------------------------------------------------

template<typename T_OUT>
bool deserializeIntegerValue(const QJsonValue inputValue, T_OUT *outputValue)
{
    if (inputValue.isDouble())
    {
        return convertIntegerValue(inputValue.toDouble(), outputValue);
    }

    if (inputValue.isString())
    {
        return convertIntegerValue(inputValue.toString(), outputValue);
    }

    qCWarning(CedarFramework::LoggingCategory::Deserialization)
            << QStringLiteral("JSON value is not a valid integer:") << inputValue;
    return false;
}

// -------------------------------------------------------------------------------------------------

bool convertFloatingPointValue(const double inputValue, float *outputValue)
{
    constexpr auto lowwerLimit = -std::numeric_limits<float>::max();
    constexpr auto upperLimit = std::numeric_limits<float>::max();

    // Make sure that the input value can be stored in the output
    if ((inputValue < static_cast<double>(lowwerLimit)) ||
        (inputValue > static_cast<double>(upperLimit)))
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QString("Parameter value [%1] is out of range for the its data type "
                           "(min: [%2], max: [%3])!")
                   .arg(inputValue, 0, 'f')
                   .arg(lowwerLimit, 0, 'f')
                   .arg(upperLimit, 0, 'f');
        return false;
    }

    *outputValue = static_cast<float>(inputValue);
    return true;
}

} // namespace Internal

// -------------------------------------------------------------------------------------------------
// Implementation
// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, bool *value)
{
    Q_ASSERT(value != nullptr);

    // From Boolean value
    if (json.isBool())
    {
        *value = json.toBool();
        return true;
    }

    // From string value
    if (json.isString())
    {
        const QString stringValue = json.toString();

        if (stringValue == QStringLiteral("0"))
        {
            *value = false;
            return true;
        }

        if (stringValue == QStringLiteral("1"))
        {
            *value = true;
            return true;
        }

        if (stringValue.compare(QStringLiteral("false"), Qt::CaseInsensitive) == 0)
        {
            *value = false;
            return true;
        }

        if (stringValue.compare(QStringLiteral("true"), Qt::CaseInsensitive) == 0)
        {
            *value = true;
            return true;
        }

        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QString("Not a valid Boolean value in string format [%1]! "
                           "Only [false], [true], [0], and [1] values are allowed!")
                   .arg(stringValue);
        return false;
    }

    // From number
    if (json.isDouble())
    {
        constexpr double limit = 1e-3;
        const double doubleValue = json.toDouble();

        // Check if value is very close to 0
        if (std::abs(doubleValue) < limit)
        {
            *value = false;
            return true;
        }

        // Check if value is very close to 1
        if (std::abs(doubleValue - 1.0) < limit)
        {
            *value = true;
            return true;
        }

        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QString("Not a valid Boolean value in floating-point format [%1]! "
                           "Only [0.0] and [1.0] values (with 1e-3 rounding error) are allowed!")
                   .arg(doubleValue);
        return false;
    }

    qCWarning(CedarFramework::LoggingCategory::Deserialization)
            << QStringLiteral("JSON value is not a valid Boolean value:") << json;
    return false;
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, signed char *value)
{
    Q_ASSERT(value != nullptr);

    return Internal::deserializeIntegerValue(json, value);
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, unsigned char *value)
{
    Q_ASSERT(value != nullptr);

    return Internal::deserializeIntegerValue(json, value);
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, short *value)
{
    Q_ASSERT(value != nullptr);

    return Internal::deserializeIntegerValue(json, value);
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, unsigned short *value)
{
    Q_ASSERT(value != nullptr);

    return Internal::deserializeIntegerValue(json, value);
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, int *value)
{
    Q_ASSERT(value != nullptr);

    return Internal::deserializeIntegerValue(json, value);
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, unsigned int *value)
{
    Q_ASSERT(value != nullptr);

    return Internal::deserializeIntegerValue(json, value);
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, long *value)
{
    Q_ASSERT(value != nullptr);

    return Internal::deserializeIntegerValue(json, value);
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, unsigned long *value)
{
    Q_ASSERT(value != nullptr);

    return Internal::deserializeIntegerValue(json, value);
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, long long *value)
{
    Q_ASSERT(value != nullptr);

    return Internal::deserializeIntegerValue(json, value);
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, unsigned long long *value)
{
    Q_ASSERT(value != nullptr);

    return Internal::deserializeIntegerValue(json, value);
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, float *value)
{
    Q_ASSERT(value != nullptr);

    if (json.isDouble())
    {
        return Internal::convertFloatingPointValue(json.toDouble(), value);
    }

    if (json.isString())
    {
        bool ok = false;
        const double doubleValue = json.toString().toDouble(&ok);

        if (!ok)
        {
            qCWarning(CedarFramework::LoggingCategory::Deserialization)
                    << QString("Value [%1] is not a valid floating-point number!")
                       .arg(json.toString());
            return false;
        }

        return Internal::convertFloatingPointValue(doubleValue, value);
    }

    qCWarning(CedarFramework::LoggingCategory::Deserialization)
            << QStringLiteral("JSON value is not a valid floating-point number:") << json;
    return false;
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, double *value)
{
    Q_ASSERT(value != nullptr);

    if (json.isDouble())
    {
        *value = json.toDouble();
        return true;
    }

    if (json.isString())
    {
        bool ok = false;
        *value = json.toString().toDouble(&ok);

        if (!ok)
        {
            qCWarning(CedarFramework::LoggingCategory::Deserialization)
                    << QString("Value [%1] is not a valid floating-point number!")
                       .arg(json.toString());
            return false;
        }

        return true;
    }

    qCWarning(CedarFramework::LoggingCategory::Deserialization)
            << QStringLiteral("JSON value is not a valid floating-point number:") << json;
    return false;
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, QChar *value)
{
    Q_ASSERT(value != nullptr);

    if (!json.isString())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("JSON value is not a valid character:") << json;
        return false;
    }

    const QString stringValue = json.toString();

    if (stringValue.size() != 1)
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("JSON value contains more than one character:") << stringValue;
        return false;
    }

    *value = stringValue.front();
    return true;
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, QString *value)
{
    Q_ASSERT(value != nullptr);

    if (!json.isString())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("JSON value is not a string:") << json;
        return false;
    }

    *value = json.toString();
    return true;
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, QByteArray *value)
{
    Q_ASSERT(value != nullptr);

    if (!json.isString())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("JSON value is not a valid byte array:") << json;
        return false;
    }

    const auto encodedData = json.toString().toLatin1();

    if (encodedData.isEmpty())
    {
        value->clear();
        return true;
    }

    *value = QByteArray::fromBase64(encodedData, QByteArray::AbortOnBase64DecodingErrors);

    if (value->isEmpty())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("JSON value is not a valid byte array:") << json;
        return false;
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, QBitArray *value)
{
    Q_ASSERT(value != nullptr);

    // Get the JSON Object representation
    if (!json.isObject())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("JSON value is not a valid bit array:") << json;
        return false;
    }

    const auto jsonObject = json.toObject();

    if (jsonObject.size() != 2)
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QString("A bit array needs to have exactly 2 members but this one has %1! JSON "
                           "value:").arg(jsonObject.size())
                << jsonObject;
        return false;
    }

    // Deserialize members
    int bitCount = 0;

    if (!deserialize(jsonObject.value(QStringLiteral("bit_count")), &bitCount))
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("Failed to deserialize the member 'bit_count' of a bit array:")
                << jsonObject;
        return false;
    }

    QByteArray encodedBits;

    if (!deserialize(jsonObject.value(QStringLiteral("encoded_bits")), &encodedBits))
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("Failed to deserialize the member 'encoded_bits' of a bit array:")
                << jsonObject;
        return false;
    }

    // Check if the bit count matches the size of the encoded bits
    int byteCount = bitCount / 8;

    if ((bitCount % 8) > 0)
    {
        byteCount++;
    }

    if (encodedBits.size() != byteCount)
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QString("The 'bit_count' [%1] doesn't correspond to the size of 'encoded_bits' "
                           "[%2] of a bit array. Expected size was [%3]! JSON value:")
                   .arg(bitCount)
                   .arg(encodedBits.size())
                   .arg(byteCount)
                << jsonObject;
        return false;
    }

    // Recreate the bit array
    *value = QBitArray::fromBits(encodedBits.constData(), bitCount);
    return true;
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, std::string *value)
{
    Q_ASSERT(value != nullptr);
    QString stringValue;

    if (!deserialize(json, &stringValue))
    {
        return false;
    }

    *value = stringValue.toStdString();
    return true;
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, std::wstring *value)
{
    Q_ASSERT(value != nullptr);
    QString stringValue;

    if (!deserialize(json, &stringValue))
    {
        return false;
    }

    *value = stringValue.toStdWString();
    return true;
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, std::u16string *value)
{
    Q_ASSERT(value != nullptr);
    QString stringValue;

    if (!deserialize(json, &stringValue))
    {
        return false;
    }

    *value = stringValue.toStdU16String();
    return true;
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, std::u32string *value)
{
    Q_ASSERT(value != nullptr);
    QString stringValue;

    if (!deserialize(json, &stringValue))
    {
        return false;
    }

    *value = stringValue.toStdU32String();
    return true;
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, QDate *value)
{
    Q_ASSERT(value != nullptr);

    if (!json.isString())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("JSON value is not a valid date:") << json;
        return false;
    }

    const QString stringValue = json.toString();

    if (stringValue.isEmpty())
    {
        *value = QDate();
        return true;
    }

    *value = QDate::fromString(stringValue, Qt::ISODate);

    if (!value->isValid())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QString("The value [%1] doesn't represent a date value in ISO format")
                   .arg(stringValue);
        return false;
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, QTime *value)
{
    Q_ASSERT(value != nullptr);

    if (!json.isString())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("JSON value is not a valid time:") << json;
        return false;
    }

    const QString stringValue = json.toString();

    if (stringValue.isEmpty())
    {
        *value = QTime();
        return true;
    }

    *value = QTime::fromString(stringValue, Qt::ISODateWithMs);

    if (!value->isValid())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QString("The value [%1] doesn't represent a time value in ISO format")
                   .arg(stringValue);
        return false;
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, QDateTime *value)
{
    Q_ASSERT(value != nullptr);

    if (!json.isString())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("JSON value is not a valid date and time:") << json;
        return false;
    }

    const QString stringValue = json.toString();

    if (stringValue.isEmpty())
    {
        *value = QDateTime();
        return true;
    }

    *value = QDateTime::fromString(stringValue, Qt::ISODateWithMs);

    if (!value->isValid())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QString("The value [%1] doesn't represent a date and time value in ISO format")
                   .arg(stringValue);
        return false;
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, QVariant *value)
{
    // TODO: reimplement this if support is added for "full encoding" (variant type and serialized
    //       value)?
    *value = json.toVariant();
    return true;
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, QUrl *value)
{
    Q_ASSERT(value != nullptr);

    if (!json.isString())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("JSON value is not a valid URL:") << json;
        return false;
    }

    const QString stringValue = json.toString();

    value->setUrl(stringValue);

    if ((!value->isValid()) && (!stringValue.isEmpty()))
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QString("The value [%1] doesn't represent an URL value")
                   .arg(stringValue);
        return false;
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, QUuid *value)
{
    Q_ASSERT(value != nullptr);

    if (!json.isString())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("JSON value is not a valid UUID:") << json;
        return false;
    }

    const QString stringValue = json.toString();

    *value = QUuid::fromString(stringValue);

    if (value->isNull() &&
        (stringValue != QStringLiteral("{00000000-0000-0000-0000-000000000000}")))
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QString("The value [%1] doesn't represent an UUID value")
                   .arg(stringValue);
        return false;
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, QLocale *value)
{
    Q_ASSERT(value != nullptr);

    if (!json.isString())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("JSON value is not a valid locale:") << json;
        return false;
    }

    *value = QLocale(json.toString());
    return true;
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, QRegExp *value)
{
    Q_ASSERT(value != nullptr);

    // Get the JSON Object representation
    if (!json.isObject())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("JSON value is not a valid QRegExp:") << json;
        return false;
    }

    const auto jsonObject = json.toObject();

    if (jsonObject.size() != 3)
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QString("A QRegExp needs to have exactly 3 members but this one has %1! JSON "
                           "value:").arg(jsonObject.size())
                << jsonObject;
        return false;
    }

    // Deserialize members
    QString pattern;

    if (!deserialize(jsonObject.value(QStringLiteral("pattern")), &pattern))
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("Failed to deserialize the member 'pattern' of a QRegExp:")
                << jsonObject;
        return false;
    }

    bool caseSensitive;

    if (!deserialize(jsonObject.value(QStringLiteral("case_sensitive")), &caseSensitive))
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("Failed to deserialize the member 'case_sensitive' of a QRegExp:")
                << jsonObject;
        return false;
    }

    QString syntax;

    if (!deserialize(jsonObject.value(QStringLiteral("syntax")), &syntax))
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("Failed to deserialize the member 'syntax' of a QRegExp:")
                << jsonObject;
        return false;
    }

    static const QHash<QString, QRegExp::PatternSyntax> syntaxMap =
    {
        { QStringLiteral("RegExp"),         QRegExp::RegExp },
        { QStringLiteral("RegExp2"),        QRegExp::RegExp2 },
        { QStringLiteral("Wildcard"),       QRegExp::Wildcard },
        { QStringLiteral("WildcardUnix"),   QRegExp::WildcardUnix },
        { QStringLiteral("FixedString"),    QRegExp::FixedString },
        { QStringLiteral("W3CXmlSchema11"), QRegExp::W3CXmlSchema11 }
    };

    if (!syntaxMap.contains(syntax))
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("Invalid 'syntax' member of the QRegExp:") << syntax;
        return false;
    }

    *value = QRegExp(pattern,
                     caseSensitive ? Qt::CaseSensitive
                                   : Qt::CaseInsensitive,
                     syntaxMap.value(syntax));
    return true;
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, QRegularExpression *value)
{
    Q_ASSERT(value != nullptr);

    // Get the JSON Object representation
    if (!json.isObject())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("JSON value is not a valid QRegularExpression:") << json;
        return false;
    }

    const auto jsonObject = json.toObject();

    if (jsonObject.size() != 2)
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QString("A QRegularExpression needs to have exactly 2 members but this one has "
                           "%1! JSON value:").arg(jsonObject.size())
                << jsonObject;
        return false;
    }

    // Deserialize members
    QString pattern;

    if (!deserialize(jsonObject.value(QStringLiteral("pattern")), &pattern))
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("Failed to deserialize the member 'pattern' of a "
                                  "QRegularExpression:")
                << jsonObject;
        return false;
    }

    QStringList options;

    if (!deserialize(jsonObject.value(QStringLiteral("options")), &options))
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("Failed to deserialize the member 'options' of a "
                                  "QRegularExpression:")
                << jsonObject;
        return false;
    }

    static const QHash<QString, QRegularExpression::PatternOption> syntaxMap =
    {
        { QStringLiteral("CaseInsensitive"),       QRegularExpression::CaseInsensitiveOption },
        { QStringLiteral("DotMatchesEverything"),  QRegularExpression::DotMatchesEverythingOption },
        { QStringLiteral("Multiline"),             QRegularExpression::MultilineOption },
        {
            QStringLiteral("ExtendedPatternSyntax"),
            QRegularExpression::ExtendedPatternSyntaxOption
        },
        { QStringLiteral("InvertedGreediness"),    QRegularExpression::InvertedGreedinessOption },
        { QStringLiteral("DontCapture"),           QRegularExpression::DontCaptureOption },
        { QStringLiteral("UseUnicodeProperties"),  QRegularExpression::UseUnicodePropertiesOption },
        { QStringLiteral("OptimizeOnFirstUsage"),  QRegularExpression::OptimizeOnFirstUsageOption },
        {
            QStringLiteral("DontAutomaticallyOptimize"),
            QRegularExpression::DontAutomaticallyOptimizeOption
        }
    };

    QRegularExpression::PatternOptions optionFlags;

    for (const QString &item : options)
    {
        if (!syntaxMap.contains(item))
        {
            qCWarning(CedarFramework::LoggingCategory::Deserialization)
                    << QString("Unsupported option [%1] in the 'options' member of a "
                               "QRegularExpression").arg(item);
            return false;
        }

        optionFlags.setFlag(syntaxMap.value(item));
    }

    *value = QRegularExpression(pattern, optionFlags);
    return true;
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, QSize *value)
{
    Q_ASSERT(value != nullptr);

    // Get the JSON Object representation
    if (!json.isObject())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("JSON value is not a valid size:") << json;
        return false;
    }

    const auto jsonObject = json.toObject();

    if (jsonObject.size() != 2)
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QString("A size needs to have exactly 2 members but this one has %1! JSON "
                           "value:").arg(jsonObject.size())
                << jsonObject;
        return false;
    }

    // Deserialize members
    int width;

    if (!deserialize(jsonObject.value(QStringLiteral("width")), &width))
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("Failed to deserialize the member 'width' of a size:")
                << jsonObject;
        return false;
    }

    int height;

    if (!deserialize(jsonObject.value(QStringLiteral("height")), &height))
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("Failed to deserialize the member 'height' of a size:")
                << jsonObject;
        return false;
    }

    value->setWidth(width);
    value->setHeight(height);
    return true;
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, QSizeF *value)
{
    Q_ASSERT(value != nullptr);

    // Get the JSON Object representation
    if (!json.isObject())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("JSON value is not a valid size:") << json;
        return false;
    }

    const auto jsonObject = json.toObject();

    if (jsonObject.size() != 2)
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QString("A size needs to have exactly 2 members but this one has %1! JSON "
                           "value:").arg(jsonObject.size())
                << jsonObject;
        return false;
    }

    // Deserialize members
    qreal width;

    if (!deserialize(jsonObject.value(QStringLiteral("width")), &width))
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("Failed to deserialize the member 'width' of a size:")
                << jsonObject;
        return false;
    }

    qreal height;

    if (!deserialize(jsonObject.value(QStringLiteral("height")), &height))
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("Failed to deserialize the member 'height' of a size:")
                << jsonObject;
        return false;
    }

    value->setWidth(width);
    value->setHeight(height);
    return true;
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, QPoint *value)
{
    Q_ASSERT(value != nullptr);

    // Get the JSON Object representation
    if (!json.isObject())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("JSON value is not a valid point:") << json;
        return false;
    }

    const auto jsonObject = json.toObject();

    if (jsonObject.size() != 2)
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QString("A point needs to have exactly 2 members but this one has %1! JSON "
                           "value:").arg(jsonObject.size())
                << jsonObject;
        return false;
    }

    // Deserialize members
    int x;

    if (!deserialize(jsonObject.value(QStringLiteral("x")), &x))
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("Failed to deserialize the member 'x' of a point:")
                << jsonObject;
        return false;
    }

    int y;

    if (!deserialize(jsonObject.value(QStringLiteral("y")), &y))
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("Failed to deserialize the member 'y' of a point:")
                << jsonObject;
        return false;
    }

    value->setX(x);
    value->setY(y);
    return true;
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, QPointF *value)
{
    Q_ASSERT(value != nullptr);

    // Get the JSON Object representation
    if (!json.isObject())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("JSON value is not a valid point:") << json;
        return false;
    }

    const auto jsonObject = json.toObject();

    if (jsonObject.size() != 2)
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QString("A point needs to have exactly 2 members but this one has %1! JSON "
                           "value:").arg(jsonObject.size())
                << jsonObject;
        return false;
    }

    // Deserialize members
    qreal x;

    if (!deserialize(jsonObject.value(QStringLiteral("x")), &x))
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("Failed to deserialize the member 'x' of a point:")
                << jsonObject;
        return false;
    }

    qreal y;

    if (!deserialize(jsonObject.value(QStringLiteral("y")), &y))
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("Failed to deserialize the member 'y' of a point:")
                << jsonObject;
        return false;
    }

    value->setX(x);
    value->setY(y);
    return true;
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, QLine *value)
{
    Q_ASSERT(value != nullptr);

    // Get the JSON Object representation
    if (!json.isObject())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("JSON value is not a valid line:") << json;
        return false;
    }

    const auto jsonObject = json.toObject();

    if (jsonObject.size() != 4)
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QString("A line needs to have exactly 4 members but this one has %1! JSON "
                           "value:").arg(jsonObject.size())
                << jsonObject;
        return false;
    }

    // Deserialize members
    int x1;

    if (!deserialize(jsonObject.value(QStringLiteral("x1")), &x1))
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("Failed to deserialize the member 'x1' of a line:")
                << jsonObject;
        return false;
    }

    int x2;

    if (!deserialize(jsonObject.value(QStringLiteral("x2")), &x2))
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("Failed to deserialize the member 'x2' of a line:")
                << jsonObject;
        return false;
    }

    int y1;

    if (!deserialize(jsonObject.value(QStringLiteral("y1")), &y1))
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("Failed to deserialize the member 'y1' of a line:")
                << jsonObject;
        return false;
    }

    int y2;

    if (!deserialize(jsonObject.value(QStringLiteral("y2")), &y2))
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("Failed to deserialize the member 'y2' of a line:")
                << jsonObject;
        return false;
    }

    value->setPoints(QPoint(x1, y1), QPoint(x2, y2));
    return true;
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, QLineF *value)
{
    Q_ASSERT(value != nullptr);

    // Get the JSON Object representation
    if (!json.isObject())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("JSON value is not a valid line:") << json;
        return false;
    }

    const auto jsonObject = json.toObject();

    if (jsonObject.size() != 4)
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QString("A line needs to have exactly 4 members but this one has %1! JSON "
                           "value:").arg(jsonObject.size())
                << jsonObject;
        return false;
    }

    // Deserialize members
    qreal x1;

    if (!deserialize(jsonObject.value(QStringLiteral("x1")), &x1))
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("Failed to deserialize the member 'x1' of a line:")
                << jsonObject;
        return false;
    }

    qreal x2;

    if (!deserialize(jsonObject.value(QStringLiteral("x2")), &x2))
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("Failed to deserialize the member 'x2' of a line:")
                << jsonObject;
        return false;
    }

    qreal y1;

    if (!deserialize(jsonObject.value(QStringLiteral("y1")), &y1))
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("Failed to deserialize the member 'y1' of a line:")
                << jsonObject;
        return false;
    }

    qreal y2;

    if (!deserialize(jsonObject.value(QStringLiteral("y2")), &y2))
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("Failed to deserialize the member 'y2' of a line:")
                << jsonObject;
        return false;
    }

    value->setPoints(QPointF(x1, y1), QPointF(x2, y2));
    return true;
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, QRect *value)
{
    Q_ASSERT(value != nullptr);

    // Get the JSON Object representation
    if (!json.isObject())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("JSON value is not a valid rectangle:") << json;
        return false;
    }

    const auto jsonObject = json.toObject();

    if (jsonObject.size() != 4)
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QString("A rectangle needs to have exactly 4 members but this one has %1! JSON "
                           "value:").arg(jsonObject.size())
                << jsonObject;
        return false;
    }

    // Deserialize members
    int x;

    if (!deserialize(jsonObject.value(QStringLiteral("x")), &x))
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("Failed to deserialize the member 'x' of a rectangle:")
                << jsonObject;
        return false;
    }

    int y;

    if (!deserialize(jsonObject.value(QStringLiteral("y")), &y))
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("Failed to deserialize the member 'y' of a rectangle:")
                << jsonObject;
        return false;
    }

    int width;

    if (!deserialize(jsonObject.value(QStringLiteral("width")), &width))
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("Failed to deserialize the member 'width' of a rectangle:")
                << jsonObject;
        return false;
    }

    int height;

    if (!deserialize(jsonObject.value(QStringLiteral("height")), &height))
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("Failed to deserialize the member 'height' of a rectangle:")
                << jsonObject;
        return false;
    }

    value->setX(x);
    value->setY(y);

    value->setWidth(width);
    value->setHeight(height);
    return true;
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, QRectF *value)
{
    Q_ASSERT(value != nullptr);

    // Get the JSON Object representation
    if (!json.isObject())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("JSON value is not a valid rectangle:") << json;
        return false;
    }

    const auto jsonObject = json.toObject();

    if (jsonObject.size() != 4)
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QString("A rectangle needs to have exactly 4 members but this one has %1! JSON "
                           "value:").arg(jsonObject.size())
                << jsonObject;
        return false;
    }

    // Deserialize members
    qreal x;

    if (!deserialize(jsonObject.value(QStringLiteral("x")), &x))
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("Failed to deserialize the member 'x' of a rectangle:")
                << jsonObject;
        return false;
    }

    qreal y;

    if (!deserialize(jsonObject.value(QStringLiteral("y")), &y))
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("Failed to deserialize the member 'y' of a rectangle:")
                << jsonObject;
        return false;
    }

    qreal width;

    if (!deserialize(jsonObject.value(QStringLiteral("width")), &width))
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("Failed to deserialize the member 'width' of a rectangle:")
                << jsonObject;
        return false;
    }

    qreal height;

    if (!deserialize(jsonObject.value(QStringLiteral("height")), &height))
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("Failed to deserialize the member 'height' of a rectangle:")
                << jsonObject;
        return false;
    }

    value->setX(x);
    value->setY(y);

    value->setWidth(width);
    value->setHeight(height);
    return true;
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, QStringList *value)
{
    Q_ASSERT(value != nullptr);

    // Get the JSON Array representation
    if (!json.isArray())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("JSON value is not a valid string list:") << json;
        return false;
    }

    const auto jsonArray = json.toArray();

    // Deserialize elements
    value->clear();
    int index = 0;

    for (const auto &item : jsonArray)
    {
        QString deserializedItem;

        if (!deserialize(item, &deserializedItem))
        {
            qCWarning(CedarFramework::LoggingCategory::Deserialization)
                    << QString("Failed to deserialize the string list element at index [%1]! JSON "
                               "value:").arg(index)
                    << jsonArray;
            return false;
        }

        value->append(deserializedItem);
        index++;
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, QJsonValue *value)
{
    Q_ASSERT(value != nullptr);

    *value = json;
    return true;
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, QJsonArray *value)
{
    Q_ASSERT(value != nullptr);

    // Get the JSON Array representation
    if (!json.isArray())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("JSON value is not a valid JSON array:") << json;
        return false;
    }

    *value = json.toArray();
    return true;
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, QJsonObject *value)
{
    Q_ASSERT(value != nullptr);

    // Get the JSON Object representation
    if (!json.isObject())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("JSON value is not a valid JSON object:") << json;
        return false;
    }

    *value = json.toObject();
    return true;
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, QJsonDocument *value)
{
    Q_ASSERT(value != nullptr);

    if (json.isNull())
    {
        *value = QJsonDocument();
        return true;
    }

    if (json.isArray())
    {
        value->setArray(json.toArray());
        return true;
    }

    if (json.isObject())
    {
        value->setObject(json.toObject());
        return true;
    }

    qCWarning(CedarFramework::LoggingCategory::Deserialization)
            << QStringLiteral("Failed to deserialize the JSON value to a JSON document:") << json;
    return false;
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, QCborValue *value)
{
    Q_ASSERT(value != nullptr);

    *value = QCborValue::fromJsonValue(json);
    return true;
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, QCborArray *value)
{
    Q_ASSERT(value != nullptr);

    // Get the JSON Array representation
    if (!json.isArray())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("JSON value is not a valid CBOR array:") << json;
        return false;
    }

    *value = QCborArray::fromJsonArray(json.toArray());
    return true;
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, QCborMap *value)
{
    Q_ASSERT(value != nullptr);

    // Get the JSON Object representation
    if (!json.isObject())
    {
        qCWarning(CedarFramework::LoggingCategory::Deserialization)
                << QStringLiteral("JSON value is not a valid CBOR map:") << json;
        return false;
    }

    *value = QCborMap::fromJsonObject(json.toObject());
    return true;
}

// -------------------------------------------------------------------------------------------------

template<>
bool deserialize(const QJsonValue &json, QCborSimpleType *value)
{
    Q_ASSERT(value != nullptr);

    if (json.isBool())
    {
        *value = json.toBool() ? QCborSimpleType::True
                               : QCborSimpleType::False;
        return true;
    }

    if (json.isNull())
    {
        *value = QCborSimpleType::Null;
        return true;
    }

    if (json.isUndefined())
    {
        *value = QCborSimpleType::Undefined;
        return true;
    }

    qCWarning(CedarFramework::LoggingCategory::Deserialization)
            << QStringLiteral("Failed to deserialize the JSON value to a CBOR simple type:")
            << json;
    return false;
}

} // namespace CedarFramework
