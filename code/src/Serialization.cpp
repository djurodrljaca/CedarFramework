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

// Own header
#include <CedarFramework/Serialization.hpp>

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

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CedarFramework
{

template<>
QJsonValue serialize(const bool &value)
{
    return value;
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const signed char &value)
{
    return value;
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const unsigned char &value)
{
    return value;
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const short &value)
{
    return value;
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const unsigned short &value)
{
    return value;
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const int &value)
{
    return value;
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const unsigned int &value)
{
    return static_cast<qint64>(value);
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const long &value)
{
    return static_cast<qint64>(value);
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const unsigned long &value)
{
    return static_cast<qint64>(value);
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const long long &value)
{
    // Store the value as integer if it can be stored without loss of precision, otherwise store it
    // as a string
    constexpr long long positiveLimit =  9007199254740992LL;
    constexpr long long negativeLimit = -9007199254740992LL;

    if ((negativeLimit <= value) && (value <= positiveLimit))
    {
        return static_cast<qint64>(value);
    }

    return QString::number(value);
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const unsigned long long &value)
{
    // Store the value as integer if it can be stored without loss of precision, otherwise store it
    // as a string
    constexpr unsigned long long limit = 9007199254740992ULL;

    if (value <= limit)
    {
        return static_cast<qint64>(value);
    }

    return QString::number(value);
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const float &value)
{
    return value;
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const double &value)
{
    return value;
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const QChar &value)
{
    return QString(value);
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const QString &value)
{
    return value;
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const QByteArray &value)
{
    if (value.isEmpty())
    {
        return QString();
    }

    return QString::fromLatin1(value.toBase64());
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const QBitArray &value)
{
    const int bitCount = value.size();
    int byteCount = bitCount / 8;

    if ((bitCount % 8) > 0)
    {
        byteCount++;
    }

    return QJsonObject {
        { QStringLiteral("bit_count"), QJsonValue(bitCount) },
        { QStringLiteral("encoded_bits"), serialize(QByteArray(value.bits(), byteCount)) }
    };
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const std::string &value)
{
    return QString::fromStdString(value);
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const std::wstring &value)
{
    return QString::fromStdWString(value);
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const std::u16string &value)
{
    return QString::fromStdU16String(value);
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const std::u32string &value)
{
    return QString::fromStdU32String(value);
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const QDate &value)
{
    return value.toString(Qt::ISODate);
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const QTime &value)
{
    return value.toString(Qt::ISODateWithMs);
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const QDateTime &value)
{
    return value.toString(Qt::ISODateWithMs);
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const QVariant &value)
{
    // Check for a compatible numeric type in the QVariant value
    switch (static_cast<QMetaType::Type>(value.type()))
    {
        case QMetaType::Nullptr:
        {
            return QJsonValue::Null;
        }

        case QMetaType::Bool:
        {
            return serialize(value.value<bool>());
        }

        case QMetaType::SChar:
        {
            return serialize(value.value<signed char>());
        }

        case QMetaType::UChar:
        {
            return serialize(value.value<unsigned char>());
        }

        case QMetaType::Short:
        {
            return serialize(value.value<short>());
        }

        case QMetaType::UShort:
        {
            return serialize(value.value<unsigned short>());
        }

        case QMetaType::Int:
        {
            return serialize(value.value<int>());
        }

        case QMetaType::UInt:
        {
            return serialize(value.value<unsigned int>());
        }

        case QMetaType::Long:
        {
            return serialize(value.value<long>());
        }

        case QMetaType::ULong:
        {
            return serialize(value.value<unsigned long>());
        }

        case QMetaType::LongLong:
        {
            return serialize(value.value<long long>());
        }

        case QMetaType::ULongLong:
        {
            return serialize(value.value<unsigned long long>());
        }

        case QMetaType::Float:
        {
            return serialize(value.value<float>());
        }

        case QMetaType::Double:
        {
            return serialize(value.value<double>());
        }

        case QMetaType::QTime:
        {
            return serialize(value.value<QTime>());
        }

        case QMetaType::QDate:
        {
            return serialize(value.value<QDate>());
        }

        case QMetaType::QDateTime:
        {
            return serialize(value.value<QDateTime>());
        }

        case QMetaType::Char:
        {
            return serialize(QString(QChar(value.value<char>())));
        }

        case QMetaType::QChar:
        {
            return serialize(QString(value.value<QChar>()));
        }

        case QMetaType::QString:
        {
            return serialize(value.value<QString>());
        }

        case QMetaType::QByteArray:
        {
            return serialize(value.value<QByteArray>());
        }

        case QMetaType::QBitArray:
        {
            return serialize(value.value<QBitArray>());
        }

        case QMetaType::QUrl:
        {
            return serialize(value.value<QUrl>());
        }

        case QMetaType::QUuid:
        {
            return serialize(value.value<QUuid>());
        }

        case QMetaType::QLocale:
        {
            return serialize(value.value<QLocale>());
        }

        case QMetaType::QRegExp:
        {
            return serialize(value.value<QRegExp>());
        }

        case QMetaType::QRegularExpression:
        {
            return serialize(value.value<QRegularExpression>());
        }

        case QMetaType::QSize:
        {
            return serialize(value.value<QSize>());
        }

        case QMetaType::QSizeF:
        {
            return serialize(value.value<QSizeF>());
        }

        case QMetaType::QPoint:
        {
            return serialize(value.value<QPoint>());
        }

        case QMetaType::QPointF:
        {
            return serialize(value.value<QPointF>());
        }

        case QMetaType::QLine:
        {
            return serialize(value.value<QLine>());
        }

        case QMetaType::QLineF:
        {
            return serialize(value.value<QLineF>());
        }

        case QMetaType::QRect:
        {
            return serialize(value.value<QRect>());
        }

        case QMetaType::QRectF:
        {
            return serialize(value.value<QRectF>());
        }

        case QMetaType::QStringList:
        {
            return serialize(value.value<QStringList>());
        }

        case QMetaType::QByteArrayList:
        {
            return serialize(value.value<QByteArrayList>());
        }

        case QMetaType::QVariantList:
        {
            return serialize(value.value<QVariantList>());
        }

        case QMetaType::QVariantMap:
        {
            return serialize(value.value<QVariantMap>());
        }

        case QMetaType::QVariantHash:
        {
            return serialize(value.value<QVariantHash>());
        }

        case QMetaType::QJsonValue:
        {
            return serialize(value.value<QJsonValue>());
        }

        case QMetaType::QJsonArray:
        {
            return serialize(value.value<QJsonArray>());
        }

        case QMetaType::QJsonObject:
        {
            return serialize(value.value<QJsonObject>());
        }

        case QMetaType::QJsonDocument:
        {
            return serialize(value.value<QJsonDocument>());
        }

        case QMetaType::QCborValue:
        {
            return serialize(value.value<QCborValue>());
        }

        case QMetaType::QCborArray:
        {
            return serialize(value.value<QCborArray>());
        }

        case QMetaType::QCborMap:
        {
            return serialize(value.value<QCborMap>());
        }

        case QMetaType::QCborSimpleType:
        {
            return serialize(value.value<QCborSimpleType>());
        }

        // Note: QVariant cannot be wrapped in another QVariant!
        case QMetaType::QVariant:
        {
            qCWarning(CedarFramework::LoggingCategory::Serialization)
                    << QStringLiteral("Cannot serialize a QVariant wrapped in another QVariant");
            return QJsonValue::Undefined;
        }

        // TODO: add support for Qt GUI types?
        case QMetaType::QBitmap:
        case QMetaType::QBrush:
        case QMetaType::QColor:
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
        case QMetaType::QColorSpace:
#endif
        case QMetaType::QCursor:
        case QMetaType::QFont:
        case QMetaType::QIcon:
        case QMetaType::QImage:
        case QMetaType::QKeySequence:
        case QMetaType::QMatrix:
        case QMetaType::QMatrix4x4:
        case QMetaType::QPalette:
        case QMetaType::QPixmap:
        case QMetaType::QQuaternion:
        case QMetaType::QRegion:
        case QMetaType::QPen:
        case QMetaType::QPolygon:
        case QMetaType::QPolygonF:
        case QMetaType::QTextFormat:
        case QMetaType::QTextLength:
        case QMetaType::QTransform:
        case QMetaType::QVector2D:
        case QMetaType::QVector3D:
        case QMetaType::QVector4D:

        // TODO: add support for Qt Widgets types?
        case QMetaType::QSizePolicy:

        // TODO: add support for handling user types?
        case QMetaType::User:

        // Note: these values cannot be serialized to JSON!
        case QMetaType::QEasingCurve:
        case QMetaType::QModelIndex:
        case QMetaType::QObjectStar:
        case QMetaType::QPersistentModelIndex:
        case QMetaType::Void:
        case QMetaType::VoidStar:
        case QMetaType::UnknownType:
        default:
        {
            qCWarning(CedarFramework::LoggingCategory::Serialization)
                    << QString("Unsupported QVariant type: %1 (%2)")
                       .arg(value.userType())
                       .arg(value.typeName());
            return QJsonValue::Undefined;
        }
    }
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const QUrl &value)
{
    return value.toString();
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const QUuid &value)
{
    return value.toString();
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const QLocale &value)
{
    return value.bcp47Name();
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const QRegExp &value)
{
    static const QHash<QRegExp::PatternSyntax, QString> syntaxMap =
    {
        { QRegExp::RegExp,         QStringLiteral("RegExp") },
        { QRegExp::RegExp2,        QStringLiteral("RegExp2") },
        { QRegExp::Wildcard,       QStringLiteral("Wildcard") },
        { QRegExp::WildcardUnix,   QStringLiteral("WildcardUnix") },
        { QRegExp::FixedString,    QStringLiteral("FixedString") },
        { QRegExp::W3CXmlSchema11, QStringLiteral("W3CXmlSchema11") }
    };

    return QJsonObject {
        { QStringLiteral("pattern"),        value.pattern() },
        { QStringLiteral("case_sensitive"), (value.caseSensitivity() == Qt::CaseSensitive) },
        { QStringLiteral("syntax"),         syntaxMap.value(value.patternSyntax()) }
    };
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const QRegularExpression &value)
{
    const auto patternOptions = value.patternOptions();
    QJsonArray options;

    if (patternOptions.testFlag(QRegularExpression::CaseInsensitiveOption))
    {
        options.append(QStringLiteral("CaseInsensitive"));
    }

    if (patternOptions.testFlag(QRegularExpression::DotMatchesEverythingOption))
    {
        options.append(QStringLiteral("DotMatchesEverything"));
    }

    if (patternOptions.testFlag(QRegularExpression::MultilineOption))
    {
        options.append(QStringLiteral("Multiline"));
    }

    if (patternOptions.testFlag(QRegularExpression::ExtendedPatternSyntaxOption))
    {
        options.append(QStringLiteral("ExtendedPatternSyntax"));
    }

    if (patternOptions.testFlag(QRegularExpression::InvertedGreedinessOption))
    {
        options.append(QStringLiteral("InvertedGreediness"));
    }

    if (patternOptions.testFlag(QRegularExpression::DontCaptureOption))
    {
        options.append(QStringLiteral("DontCapture"));
    }

    if (patternOptions.testFlag(QRegularExpression::UseUnicodePropertiesOption))
    {
        options.append(QStringLiteral("UseUnicodeProperties"));
    }

    if (patternOptions.testFlag(QRegularExpression::OptimizeOnFirstUsageOption))
    {
        options.append(QStringLiteral("OptimizeOnFirstUsage"));
    }

    if (patternOptions.testFlag(QRegularExpression::DontAutomaticallyOptimizeOption))
    {
        options.append(QStringLiteral("DontAutomaticallyOptimize"));
    }

    return QJsonObject {
        { QStringLiteral("pattern"), serialize(value.pattern()) },
        { QStringLiteral("options"), serialize(options) }
    };
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const QSize &value)
{
    return QJsonObject {
        { QStringLiteral("width"),  serialize(value.width()) },
        { QStringLiteral("height"), serialize(value.height()) }
    };
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const QSizeF &value)
{
    return QJsonObject {
        { QStringLiteral("width"),  serialize(value.width()) },
        { QStringLiteral("height"), serialize(value.height()) }
    };
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const QPoint &value)
{
    return QJsonObject {
        { QStringLiteral("x"), serialize(value.x()) },
        { QStringLiteral("y"), serialize(value.y()) }
    };
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const QPointF &value)
{
    return QJsonObject {
        { QStringLiteral("x"), serialize(value.x()) },
        { QStringLiteral("y"), serialize(value.y()) }
    };
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const QLine &value)
{
    return QJsonObject {
        { QStringLiteral("x1"), serialize(value.x1()) },
        { QStringLiteral("y1"), serialize(value.y1()) },

        { QStringLiteral("x2"), serialize(value.x2()) },
        { QStringLiteral("y2"), serialize(value.y2()) }
    };
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const QLineF &value)
{
    return QJsonObject {
        { QStringLiteral("x1"), serialize(value.x1()) },
        { QStringLiteral("y1"), serialize(value.y1()) },

        { QStringLiteral("x2"), serialize(value.x2()) },
        { QStringLiteral("y2"), serialize(value.y2()) }
    };
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const QRect &value)
{
    return QJsonObject {
        { QStringLiteral("x"), serialize(value.x()) },
        { QStringLiteral("y"), serialize(value.y()) },

        { QStringLiteral("width"),  serialize(value.width()) },
        { QStringLiteral("height"), serialize(value.height()) }
    };
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const QRectF &value)
{
    return QJsonObject {
        { QStringLiteral("x"), serialize(value.x()) },
        { QStringLiteral("y"), serialize(value.y()) },

        { QStringLiteral("width"),  serialize(value.width()) },
        { QStringLiteral("height"), serialize(value.height()) }
    };
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const QStringList &value)
{
    return QJsonArray::fromStringList(value);
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const QJsonValue &value)
{
    return value;
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const QJsonArray &value)
{
    return value;
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const QJsonObject &value)
{
    return value;
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const QJsonDocument &value)
{
    if (value.isArray())
    {
        return value.array();
    }

    if (value.isObject())
    {
        return value.object();
    }

    return QJsonValue::Null;
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const QCborValue &value)
{
    return value.toJsonValue();
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const QCborArray &value)
{
    return value.toJsonArray();
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const QCborMap &value)
{
    return value.toJsonObject();
}

// -------------------------------------------------------------------------------------------------

template<>
QJsonValue serialize(const QCborSimpleType &value)
{
    switch (value)
    {
        case QCborSimpleType::False:
        {
            return false;
        }

        case QCborSimpleType::True:
        {
            return true;
        }

        case QCborSimpleType::Null:
        {
            return QJsonValue::Null;
        }

        case QCborSimpleType::Undefined:
        default:
        {
            return QJsonValue::Undefined;
        }
    }
}

} // namespace CedarFramework
