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
 * Contains unit tests for Deserialization class
 */

// Cedar Framework includes
#include <CedarFramework/Deserialization.hpp>

// Qt includes
#include <QtCore/QBitArray>
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
#include <QtCore/QCborArray>
#include <QtCore/QCborMap>
#include <QtCore/QCborValue>
#endif
#include <QtCore/QDebug>
#include <QtCore/QJsonDocument>
#include <QtCore/QLine>
#include <QtCore/QRegularExpression>
#include <QtTest/QTest>

// System includes

// Forward declarations

// Macros

// Test class declaration --------------------------------------------------------------------------

template<typename T>
using IsMax32BitInteger = std::enable_if_t<std::is_integral<T>::value && (sizeof(T) <= 4), bool>;

template<typename T>
using Is64BitInteger = std::enable_if_t<std::is_integral<T>::value && (sizeof(T) == 8), bool>;

// -------------------------------------------------------------------------------------------------

template<typename T, IsMax32BitInteger<T> = true>
void createIntegerTestData()
{
    // Check if input value is signed
    if (std::is_signed<T>::value)
    {
        // Positive tests
        QTest::newRow("Double: min")
                << QJsonValue(static_cast<double>(std::numeric_limits<T>::lowest()))
                << std::numeric_limits<T>::lowest()
                << true;
        QTest::newRow("Double: -1") << QJsonValue(-1.0) << static_cast<T>(-1) << true;
        QTest::newRow("Double:  0") << QJsonValue( 0.0) << static_cast<T>( 0) << true;
        QTest::newRow("Double: +1") << QJsonValue( 1.0) << static_cast<T>( 1) << true;
        QTest::newRow("Double: max")
                << QJsonValue(static_cast<double>(std::numeric_limits<T>::max()))
                << std::numeric_limits<T>::max()
                << true;

        QTest::newRow("String: min")
                << QJsonValue(QString::number(std::numeric_limits<T>::lowest()))
                << std::numeric_limits<T>::lowest()
                << true;
        QTest::newRow("String: -1")
                << QJsonValue(QString::number(-1.0)) << static_cast<T>(-1) << true;
        QTest::newRow("String:  0")
                << QJsonValue(QString::number( 0.0)) << static_cast<T>( 0) << true;
        QTest::newRow("String: +1")
                << QJsonValue(QString::number( 1.0)) << static_cast<T>( 1) << true;
        QTest::newRow("String: max")
                << QJsonValue(QString::number(std::numeric_limits<T>::max()))
                << std::numeric_limits<T>::max()
                << true;

        // Negative tests
        QTest::newRow("Null") << QJsonValue() << static_cast<T>(0) << false;

        QTest::newRow("Bool: true") << QJsonValue(true) << static_cast<T>(0) << false;
        QTest::newRow("Bool: false") << QJsonValue(false) << static_cast<T>(0) << false;

        QTest::newRow("Double: min - 1")
                << QJsonValue(static_cast<double>(std::numeric_limits<T>::lowest()) - 1.0)
                << static_cast<T>(0)
                << false;
        QTest::newRow("Double: max + 1")
                << QJsonValue(static_cast<double>(std::numeric_limits<T>::max()) + 1.0)
                << static_cast<T>(0)
                << false;

        QTest::newRow("String (from double): min - 1")
                << QJsonValue(QString::number(
                                  static_cast<double>(std::numeric_limits<T>::lowest()) - 1.0, 'f'))
                << static_cast<T>(0)
                << false;
        QTest::newRow("String (from double): max + 1")
                << QJsonValue(QString::number(
                                  static_cast<double>(std::numeric_limits<T>::max()) + 1.0, 'f'))
                << static_cast<T>(0)
                << false;

        QTest::newRow("String (from int): min - 1")
                << QJsonValue(QString::number(
                                  static_cast<qint64>(std::numeric_limits<T>::lowest()) - 1LL))
                << static_cast<T>(0)
                << false;
        QTest::newRow("String (from int): max + 1")
                << QJsonValue(QString::number(
                                  static_cast<qint64>(std::numeric_limits<T>::max()) + 1LL))
                << static_cast<T>(0)
                << false;

        QTest::newRow("String: invalid") << QJsonValue("abc") << static_cast<T>(0) << false;

        QTest::newRow("Array") << QJsonValue(QJsonArray()) << static_cast<T>(0) << false;
        QTest::newRow("Object") << QJsonValue(QJsonObject()) << static_cast<T>(0) << false;
    }
    else
    {
        // Positive tests
        QTest::newRow("Double: 0") << QJsonValue(0.0) << static_cast<T>(0) << true;
        QTest::newRow("Double: 1") << QJsonValue(1.0) << static_cast<T>(1) << true;
        QTest::newRow("Double: max")
                << QJsonValue(static_cast<double>(std::numeric_limits<T>::max()))
                << std::numeric_limits<T>::max()
                << true;

        QTest::newRow("String: 0") << QJsonValue(QString::number(0.0)) << static_cast<T>(0) << true;
        QTest::newRow("String: 1") << QJsonValue(QString::number(1.0)) << static_cast<T>(1) << true;
        QTest::newRow("String: max")
                << QJsonValue(QString::number(std::numeric_limits<T>::max()))
                << std::numeric_limits<T>::max()
                << true;

        // Negative tests
        QTest::newRow("Null") << QJsonValue() << static_cast<T>(0) << false;

        QTest::newRow("Bool: true") << QJsonValue(true) << static_cast<T>(0) << false;
        QTest::newRow("Bool: false") << QJsonValue(false) << static_cast<T>(0) << false;

        QTest::newRow("Double: -1") << QJsonValue(-1.0) << static_cast<T>(0) << false;
        QTest::newRow("Double: max + 1")
                << QJsonValue(static_cast<double>(std::numeric_limits<T>::max()) + 1.0)
                << static_cast<T>(0)
                << false;

        QTest::newRow("String (from double): -1") << QJsonValue("-1.0") << static_cast<T>(0) << false;
        QTest::newRow("String (from double): max + 1")
                << QJsonValue(QString::number(
                                  static_cast<double>(std::numeric_limits<T>::max()) + 1.0, 'f'))
                << static_cast<T>(0)
                << false;

        QTest::newRow("String (from int): -1") << QJsonValue("-1.0") << static_cast<T>(0) << false;
        QTest::newRow("String (from int): max + 1")
                << QJsonValue(QString::number(
                                  static_cast<qint64>(std::numeric_limits<T>::max()) + 1LL))
                << static_cast<T>(0)
                << false;

        QTest::newRow("String: invalid") << QJsonValue("abc") << static_cast<T>(0) << false;

        QTest::newRow("Array") << QJsonValue(QJsonArray()) << static_cast<T>(0) << false;
        QTest::newRow("Object") << QJsonValue(QJsonObject()) << static_cast<T>(0) << false;
    }
}

// -------------------------------------------------------------------------------------------------

template<typename T, Is64BitInteger<T> = true>
void createIntegerTestData()
{
    // Check if input value is signed
    if (std::is_signed<T>::value)
    {
        // Positive tests
        QTest::newRow("Double: -9007199254740992")
                << QJsonValue(-9007199254740992.0) << static_cast<T>(-9007199254740992LL) << true;
        QTest::newRow("Double: -1") << QJsonValue(-1.0) << static_cast<T>(-1) << true;
        QTest::newRow("Double:  0") << QJsonValue( 0.0) << static_cast<T>( 0) << true;
        QTest::newRow("Double: +1") << QJsonValue( 1.0) << static_cast<T>( 1) << true;
        QTest::newRow("Double: +9007199254740992")
                << QJsonValue(9007199254740992.0) << static_cast<T>(9007199254740992LL) << true;

        QTest::newRow("String: min")
                << QJsonValue(QString::number(std::numeric_limits<T>::lowest()))
                << std::numeric_limits<T>::lowest()
                << true;
        QTest::newRow("String: -9007199254740993") << QJsonValue(QString("-9007199254740993"))
                                                   << static_cast<T>(-9007199254740993LL)
                                                   << true;
        QTest::newRow("String: -9007199254740992") << QJsonValue(QString("-9007199254740992"))
                                                   << static_cast<T>(-9007199254740992LL)
                                                   << true;
        QTest::newRow("String: -1")
                << QJsonValue(QString::number(-1.0)) << static_cast<T>(-1) << true;
        QTest::newRow("String:  0")
                << QJsonValue(QString::number( 0.0)) << static_cast<T>( 0) << true;
        QTest::newRow("String: +1")
                << QJsonValue(QString::number( 1.0)) << static_cast<T>( 1) << true;
        QTest::newRow("String: +9007199254740992") << QJsonValue(QString("9007199254740992"))
                                                   << static_cast<T>(9007199254740992LL)
                                                   << true;
        QTest::newRow("String: +9007199254740993") << QJsonValue(QString("-9007199254740993"))
                                                   << static_cast<T>(-9007199254740993LL)
                                                   << true;
        QTest::newRow("String: max")
                << QJsonValue(QString::number(std::numeric_limits<T>::max()))
                << std::numeric_limits<T>::max()
                << true;

        // Negative tests
        QTest::newRow("Null") << QJsonValue() << static_cast<T>(0) << false;

        QTest::newRow("Bool: true") << QJsonValue(true) << static_cast<T>(0) << false;
        QTest::newRow("Bool: false") << QJsonValue(false) << static_cast<T>(0) << false;

        QTest::newRow("Double: min * 2")
                << QJsonValue(static_cast<double>(std::numeric_limits<T>::lowest()) * 2.0)
                << static_cast<T>(0)
                << false;
        QTest::newRow("Double: max * 2")
                << QJsonValue(static_cast<double>(std::numeric_limits<T>::max()) * 2.0)
                << static_cast<T>(0)
                << false;

        QTest::newRow("String (from double): min * 2")
                << QJsonValue(QString::number(
                                  static_cast<double>(std::numeric_limits<T>::lowest()) * 2.0, 'f'))
                << static_cast<T>(0)
                << false;
        QTest::newRow("String (from double): max * 2")
                << QJsonValue(QString::number(
                                  static_cast<double>(std::numeric_limits<T>::max()) * 2.0, 'f'))
                << static_cast<T>(0)
                << false;

        QTest::newRow("String (from uint): max + 1")
                << QJsonValue(QString::number(
                                  static_cast<quint64>(std::numeric_limits<T>::max()) + 1))
                << static_cast<T>(0)
                << false;

        QTest::newRow("String: invalid") << QJsonValue("abc") << static_cast<T>(0) << false;

        QTest::newRow("Array") << QJsonValue(QJsonArray()) << static_cast<T>(0) << false;
        QTest::newRow("Object") << QJsonValue(QJsonObject()) << static_cast<T>(0) << false;
    }
    else
    {
        // Positive tests
        QTest::newRow("Double:  0") << QJsonValue( 0.0) << static_cast<T>( 0) << true;
        QTest::newRow("Double: +1") << QJsonValue( 1.0) << static_cast<T>( 1) << true;
        QTest::newRow("Double: +9007199254740992")
                << QJsonValue(9007199254740992.0) << static_cast<T>(9007199254740992LL) << true;

        QTest::newRow("String:  0")
                << QJsonValue(QString::number( 0.0)) << static_cast<T>( 0) << true;
        QTest::newRow("String: +1")
                << QJsonValue(QString::number( 1.0)) << static_cast<T>( 1) << true;
        QTest::newRow("String: +9007199254740992") << QJsonValue(QString("9007199254740992"))
                                                   << static_cast<T>(9007199254740992ULL)
                                                   << true;
        QTest::newRow("String: +9007199254740993") << QJsonValue(QString("9007199254740993"))
                                                   << static_cast<T>(9007199254740993ULL)
                                                   << true;
        QTest::newRow("String: max")
                << QJsonValue(QString::number(std::numeric_limits<T>::max()))
                << std::numeric_limits<T>::max()
                << true;

        // Negative tests
        QTest::newRow("Null") << QJsonValue() << static_cast<T>(0) << false;

        QTest::newRow("Double: -1") << QJsonValue(-1.0) << static_cast<T>(0) << false;
        QTest::newRow("Double: max * 2")
                << QJsonValue(static_cast<double>(std::numeric_limits<T>::max()) * 2.0)
                << static_cast<T>(0)
                << false;

        QTest::newRow("String (from double): -1")
                << QJsonValue("-1.0") << static_cast<T>(0) << false;
        QTest::newRow("String (from double): max * 2")
                << QJsonValue(QString::number(
                                  static_cast<double>(std::numeric_limits<T>::max()) * 2.0, 'f'))
                << static_cast<T>(0)
                << false;

        QTest::newRow("String (from int): -1")
                << QJsonValue("-1") << static_cast<T>(0) << false;

        QTest::newRow("String: invalid") << QJsonValue("abc") << static_cast<T>(0) << false;

        QTest::newRow("Array") << QJsonValue(QJsonArray()) << static_cast<T>(0) << false;
        QTest::newRow("Object") << QJsonValue(QJsonObject()) << static_cast<T>(0) << false;
    }
}

// -------------------------------------------------------------------------------------------------

class TestDeserialization : public QObject
{
    Q_OBJECT

private slots:
    // Functions executed by QtTest before and after test suite
    void initTestCase();
    void cleanupTestCase();

    // Functions executed by QtTest before and after each test
    void init();
    void cleanup();

    // Test functions
    void testDeserializeBool();
    void testDeserializeBool_data();

    void testDeserializeSignedChar();
    void testDeserializeSignedChar_data();

    void testDeserializeUnsignedChar();
    void testDeserializeUnsignedChar_data();

    void testDeserializeShort();
    void testDeserializeShort_data();

    void testDeserializeUnsignedShort();
    void testDeserializeUnsignedShort_data();

    void testDeserializeInt();
    void testDeserializeInt_data();

    void testDeserializeUnsignedInt();
    void testDeserializeUnsignedInt_data();

    void testDeserializeLong();
    void testDeserializeLong_data();

    void testDeserializeUnsignedLong();
    void testDeserializeUnsignedLong_data();

    void testDeserializeLongLong();
    void testDeserializeLongLong_data();

    void testDeserializeUnsignedLongLong();
    void testDeserializeUnsignedLongLong_data();

    void testDeserializeFloat();
    void testDeserializeFloat_data();

    void testDeserializeDouble();
    void testDeserializeDouble_data();

    void testDeserializeQChar();
    void testDeserializeQChar_data();

    void testDeserializeQString();
    void testDeserializeQString_data();

    void testDeserializeQByteArray();
    void testDeserializeQByteArray_data();

    void testDeserializeQBitArray();
    void testDeserializeQBitArray_data();

    void testDeserializeStdString();
    void testDeserializeStdString_data();

    void testDeserializeStdWString();
    void testDeserializeStdWString_data();

    void testDeserializeStdU16String();
    void testDeserializeStdU16String_data();

    void testDeserializeStdU32String();
    void testDeserializeStdU32String_data();

    void testDeserializeQDate();
    void testDeserializeQDate_data();

    void testDeserializeQTime();
    void testDeserializeQTime_data();

    void testDeserializeQDateTime();
    void testDeserializeQDateTime_data();

    void testDeserializeQVariant();
    void testDeserializeQVariant_data();

    void testDeserializeQUrl();
    void testDeserializeQUrl_data();

    void testDeserializeQUuid();
    void testDeserializeQUuid_data();

    void testDeserializeQLocale();
    void testDeserializeQLocale_data();

    void testDeserializeQRegExp();
    void testDeserializeQRegExp_data();

    void testDeserializeQRegularExpression();
    void testDeserializeQRegularExpression_data();

    void testDeserializeQSize();
    void testDeserializeQSize_data();

    void testDeserializeQSizeF();
    void testDeserializeQSizeF_data();

    void testDeserializeQPoint();
    void testDeserializeQPoint_data();

    void testDeserializeQPointF();
    void testDeserializeQPointF_data();

    void testDeserializeQLine();
    void testDeserializeQLine_data();

    void testDeserializeQLineF();
    void testDeserializeQLineF_data();

    void testDeserializeQRect();
    void testDeserializeQRect_data();

    void testDeserializeQRectF();
    void testDeserializeQRectF_data();

    void testDeserializeQStringList();
    void testDeserializeQStringList_data();

    void testDeserializeQJsonValue();
    void testDeserializeQJsonValue_data();

    void testDeserializeQJsonArray();
    void testDeserializeQJsonArray_data();

    void testDeserializeQJsonObject();
    void testDeserializeQJsonObject_data();

    void testDeserializeQJsonDocument();
    void testDeserializeQJsonDocument_data();

#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
    void testDeserializeQCborValue();
    void testDeserializeQCborValue_data();
#endif

#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
    void testDeserializeQCborArray();
    void testDeserializeQCborArray_data();
#endif

#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
    void testDeserializeQCborMap();
    void testDeserializeQCborMap_data();
#endif

#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
    void testDeserializeQCborSimpleType();
    void testDeserializeQCborSimpleType_data();
#endif

    void testDeserializeQPair();
    void testDeserializeQPair_data();

    void testDeserializeStdPair();
    void testDeserializeStdPair_data();

    void testDeserializeQList();
    void testDeserializeQList_data();

    void testDeserializeStdList();
    void testDeserializeStdList_data();

    void testDeserializeQVector();
    void testDeserializeQVector_data();

    void testDeserializeStdVector();
    void testDeserializeStdVector_data();

    void testDeserializeQSet();
    void testDeserializeQSet_data();

    void testDeserializeQMap();
    void testDeserializeQMap_data();

    void testDeserializeStdMap();
    void testDeserializeStdMap_data();

    void testDeserializeQHash();
    void testDeserializeQHash_data();

    void testDeserializeStdUnorderedMap();
    void testDeserializeStdUnorderedMap_data();

    void testDeserializeQMultiMap();
    void testDeserializeQMultiMap_data();

    void testDeserializeQMultiHash();
    void testDeserializeQMultiHash_data();

    void testDeserializeNodeByIndex();
    void testDeserializeNodeByIndex_data();

    void testDeserializeNodeByName();
    void testDeserializeNodeByName_data();

    void testDeserializeNodeByPathVariantList();
    void testDeserializeNodeByPathVariantList_data();

    void testDeserializeNodeByPathStringList();
    void testDeserializeNodeByPathStringList_data();

    void testDeserializeOptionalNodeByIndex();
    void testDeserializeOptionalNodeByIndex_data();

    void testDeserializeOptionalNodeByName();
    void testDeserializeOptionalNodeByName_data();

    void testDeserializeOptionalNodeByPathVariantList();
    void testDeserializeOptionalNodeByPathVariantList_data();

    void testDeserializeOptionalNodeByPathStringList();
    void testDeserializeOptionalNodeByPathStringList_data();
};

// Test Case init/cleanup methods ------------------------------------------------------------------

void TestDeserialization::initTestCase()
{
}

void TestDeserialization::cleanupTestCase()
{
}

// Test init/cleanup methods -----------------------------------------------------------------------

void TestDeserialization::init()
{
}

void TestDeserialization::cleanup()
{
}

// Test: deserialize<bool>() method ----------------------------------------------------------------

void TestDeserialization::testDeserializeBool()
{
    QFETCH(QJsonValue, input);
    QFETCH(bool, expectedOutput);
    QFETCH(bool, expectedResult);

    bool output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeBool_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<bool>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    QTest::newRow("Bool: true") << QJsonValue(true) << true << true;
    QTest::newRow("Bool: false") << QJsonValue(false) << false << true;

    QTest::newRow("String: 1") << QJsonValue("1") << true << true;
    QTest::newRow("String: 0") << QJsonValue("0") << false << true;

    QTest::newRow("String: true") << QJsonValue("true") << true << true;
    QTest::newRow("String: True") << QJsonValue("True") << true << true;
    QTest::newRow("String: TRUE") << QJsonValue("TRUE") << true << true;
    QTest::newRow("String: trUE") << QJsonValue("trUE") << true << true;

    QTest::newRow("String: false") << QJsonValue("false") << false << true;
    QTest::newRow("String: False") << QJsonValue("False") << false << true;
    QTest::newRow("String: FALSE") << QJsonValue("FALSE") << false << true;
    QTest::newRow("String: falSE") << QJsonValue("falSE") << false << true;

    QTest::newRow("Double: 1.0") << QJsonValue(1.0) << true << true;
    QTest::newRow("Double: 0.0") << QJsonValue(0.0) << false << true;

    // Negative tests
    QTest::newRow("Null") << QJsonValue() << false << false;

    QTest::newRow("String: 1.0") << QJsonValue("1.0") << false << false;
    QTest::newRow("String: 0.0") << QJsonValue("0.0") << false << false;

    QTest::newRow("String: T") << QJsonValue("T") << false << false;
    QTest::newRow("String: F") << QJsonValue("F") << false << false;

    QTest::newRow("Double: 1.1") << QJsonValue(1.1) << false << false;
    QTest::newRow("Double: 0.1") << QJsonValue(0.1) << false << false;

    QTest::newRow("Array") << QJsonValue(QJsonArray()) << false << false;
    QTest::newRow("Object") << QJsonValue(QJsonObject()) << false << false;
}

// Test: deserialize<signed char>() method ---------------------------------------------------------

void TestDeserialization::testDeserializeSignedChar()
{
    QFETCH(QJsonValue, input);
    QFETCH(signed char, expectedOutput);
    QFETCH(bool, expectedResult);

    signed char output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeSignedChar_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<signed char>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    createIntegerTestData<signed char>();
}

// Test: deserialize<unsigned char>() method -------------------------------------------------------

void TestDeserialization::testDeserializeUnsignedChar()
{
    QFETCH(QJsonValue, input);
    QFETCH(unsigned char, expectedOutput);
    QFETCH(bool, expectedResult);

    unsigned char output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeUnsignedChar_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<unsigned char>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    createIntegerTestData<unsigned char>();
}

// Test: deserialize<short>() method ---------------------------------------------------------------

void TestDeserialization::testDeserializeShort()
{
    QFETCH(QJsonValue, input);
    QFETCH(short, expectedOutput);
    QFETCH(bool, expectedResult);

    short output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeShort_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<short>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    createIntegerTestData<short>();
}

// Test: deserialize<unsigned short>() method ------------------------------------------------------

void TestDeserialization::testDeserializeUnsignedShort()
{
    QFETCH(QJsonValue, input);
    QFETCH(unsigned short, expectedOutput);
    QFETCH(bool, expectedResult);

    unsigned short output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeUnsignedShort_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<unsigned short>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    createIntegerTestData<unsigned short>();
}

// Test: deserialize<int>() method -----------------------------------------------------------------

void TestDeserialization::testDeserializeInt()
{
    QFETCH(QJsonValue, input);
    QFETCH(int, expectedOutput);
    QFETCH(bool, expectedResult);

    int output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeInt_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<int>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    createIntegerTestData<int>();
}

// Test: deserialize<unsigned int>() method --------------------------------------------------------

void TestDeserialization::testDeserializeUnsignedInt()
{
    QFETCH(QJsonValue, input);
    QFETCH(unsigned int, expectedOutput);
    QFETCH(bool, expectedResult);

    unsigned int output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeUnsignedInt_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<unsigned int>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    createIntegerTestData<unsigned int>();
}

// Test: deserialize<long>() method ----------------------------------------------------------------

void TestDeserialization::testDeserializeLong()
{
    QFETCH(QJsonValue, input);
    QFETCH(long, expectedOutput);
    QFETCH(bool, expectedResult);

    long output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeLong_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<long>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    createIntegerTestData<long>();
}

// Test: deserialize<unsigned long>() method -------------------------------------------------------

void TestDeserialization::testDeserializeUnsignedLong()
{
    QFETCH(QJsonValue, input);
    QFETCH(unsigned long, expectedOutput);
    QFETCH(bool, expectedResult);

    unsigned long output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeUnsignedLong_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<unsigned long>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    createIntegerTestData<unsigned long>();
}

// Test: deserialize<long long>() method -----------------------------------------------------------

void TestDeserialization::testDeserializeLongLong()
{
    QFETCH(QJsonValue, input);
    QFETCH(long long, expectedOutput);
    QFETCH(bool, expectedResult);

    long long output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeLongLong_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<long long>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    createIntegerTestData<long long>();
}

// Test: deserialize<unsigned long long>() method --------------------------------------------------

void TestDeserialization::testDeserializeUnsignedLongLong()
{
    QFETCH(QJsonValue, input);
    QFETCH(unsigned long long, expectedOutput);
    QFETCH(bool, expectedResult);

    unsigned long long output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeUnsignedLongLong_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<unsigned long long>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    createIntegerTestData<unsigned long long>();
}

// Test: deserialize<float>() method ---------------------------------------------------------------

void TestDeserialization::testDeserializeFloat()
{
    QFETCH(QJsonValue, input);
    QFETCH(float, expectedOutput);
    QFETCH(bool, expectedResult);

    float output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeFloat_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<float>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    QTest::newRow("Double: min") << QJsonValue(std::numeric_limits<float>::lowest())
                                 << std::numeric_limits<float>::lowest()
                                 << true;
    QTest::newRow("Double: -1.0") << QJsonValue(-1.0) << -1.0F << true;
    QTest::newRow("Double:  0.0") << QJsonValue( 0.0) <<  0.0F << true;
    QTest::newRow("Double: +1.0") << QJsonValue( 1.0) <<  1.0F << true;
    QTest::newRow("Double: max") << QJsonValue(std::numeric_limits<float>::max())
                                 << std::numeric_limits<float>::max()
                                 << true;

    QTest::newRow("String: min")
            << QJsonValue(QString::number(std::numeric_limits<float>::lowest(), 0, 'f'))
            << std::numeric_limits<float>::lowest()
            << true;
    QTest::newRow("String: -1.0") << QJsonValue("-1.0") << -1.0F << true;
    QTest::newRow("String:  0.0") << QJsonValue("0.0") << 0.0F << true;
    QTest::newRow("String: +1.0") << QJsonValue("1.0") << 1.0F << true;
    QTest::newRow("String: max")
            << QJsonValue(QString::number(std::numeric_limits<float>::max(), 0, 'f'))
            << std::numeric_limits<float>::max()
            << true;


    // Negative tests
    QTest::newRow("Null") << QJsonValue() << 0.0F << false;

    QTest::newRow("Bool: true") << QJsonValue(true) << 0.0F << false;
    QTest::newRow("Bool: false") << QJsonValue(false) << 0.0F << false;

    QTest::newRow("Double: min * 2")
            << QJsonValue(std::numeric_limits<float>::lowest() * 2.0) << 0.0F << false;
    QTest::newRow("Double: max * 2")
            << QJsonValue(std::numeric_limits<float>::max() * 2.0) << 0.0F << false;

    QTest::newRow("String: min * 2")
            << QJsonValue(QString::number(std::numeric_limits<float>::lowest() * 2.0, 'f'))
            << 0.0F
            << false;
    QTest::newRow("String: max * 2")
            << QJsonValue(QString::number(std::numeric_limits<float>::max() * 2.0, 'f'))
            << 0.0F
            << false;

    QTest::newRow("String: invalid") << QJsonValue("abc") << 0.0F << false;

    QTest::newRow("Array") << QJsonValue(QJsonArray()) << 0.0F << false;
    QTest::newRow("Object") << QJsonValue(QJsonObject()) << 0.0F << false;
}

// Test: deserialize<double>() method --------------------------------------------------------------

void TestDeserialization::testDeserializeDouble()
{
    QFETCH(QJsonValue, input);
    QFETCH(double, expectedOutput);
    QFETCH(bool, expectedResult);

    double output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeDouble_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<double>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    QTest::newRow("Double: min") << QJsonValue(std::numeric_limits<double>::lowest())
                                 << std::numeric_limits<double>::lowest()
                                 << true;
    QTest::newRow("Double: -1.0") << QJsonValue(-1.0) << -1.0 << true;
    QTest::newRow("Double:  0.0") << QJsonValue( 0.0) <<  0.0 << true;
    QTest::newRow("Double: +1.0") << QJsonValue( 1.0) <<  1.0 << true;
    QTest::newRow("Double: max") << QJsonValue(std::numeric_limits<double>::max())
                                 << std::numeric_limits<double>::max()
                                 << true;

    QTest::newRow("String: min")
            << QJsonValue(QString::number(std::numeric_limits<double>::lowest(), 0, 'f'))
            << std::numeric_limits<double>::lowest()
            << true;
    QTest::newRow("String: -1.0") << QJsonValue("-1.0") << -1.0 << true;
    QTest::newRow("String:  0.0") << QJsonValue("0.0") << 0.0 << true;
    QTest::newRow("String: +1.0") << QJsonValue("1.0") << 1.0 << true;
    QTest::newRow("String: max")
            << QJsonValue(QString::number(std::numeric_limits<double>::max(), 0, 'f'))
            << std::numeric_limits<double>::max()
            << true;

    // Negative tests
    QTest::newRow("Null") << QJsonValue() << 0.0 << false;

    QTest::newRow("Bool: true") << QJsonValue(true) << 0.0 << false;
    QTest::newRow("Bool: false") << QJsonValue(false) << 0.0 << false;

    QTest::newRow("String: invalid") << QJsonValue("abc") << 0.0 << false;

    QTest::newRow("Array") << QJsonValue(QJsonArray()) << 0.0 << false;
    QTest::newRow("Object") << QJsonValue(QJsonObject()) << 0.0 << false;
}

// Test: deserialize<QChar>() method ---------------------------------------------------------------

void TestDeserialization::testDeserializeQChar()
{
    QFETCH(QJsonValue, input);
    QFETCH(QChar, expectedOutput);
    QFETCH(bool, expectedResult);

    QChar output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeQChar_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QChar>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    QTest::newRow("String: A") << QJsonValue("A") << QChar('A') << true;
    QTest::newRow("String: z") << QJsonValue("z") << QChar('z') << true;
    QTest::newRow("String: 0") << QJsonValue("0") << QChar('0') << true;
    QTest::newRow("String: 9") << QJsonValue("9") << QChar('9') << true;
    QTest::newRow("String: +") << QJsonValue("+") << QChar('+') << true;

    // Negative tests
    QTest::newRow("Null") << QJsonValue() << QChar() << false;

    QTest::newRow("Double: 1.0") << QJsonValue(1.0) << QChar() << false;

    QTest::newRow("Bool: true") << QJsonValue(true) << QChar() << false;
    QTest::newRow("Bool: false") << QJsonValue(false) << QChar() << false;

    QTest::newRow("String: empty") << QJsonValue("") << QChar() << false;
    QTest::newRow("String: multi") << QJsonValue("abc") << QChar() << false;

    QTest::newRow("Array") << QJsonValue(QJsonArray()) << QChar() << false;
    QTest::newRow("Object") << QJsonValue(QJsonObject()) << QChar() << false;
}

// Test: deserialize<QString>() method -------------------------------------------------------------

void TestDeserialization::testDeserializeQString()
{
    QFETCH(QJsonValue, input);
    QFETCH(QString, expectedOutput);
    QFETCH(bool, expectedResult);

    QString output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeQString_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QString>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    QTest::newRow("String: empty") << QJsonValue(QString()) << QString() << true;
    QTest::newRow("String: asd") << QJsonValue("asd") << QString("asd") << true;

    // Negative tests
    QTest::newRow("Null") << QJsonValue() << QString() << false;

    QTest::newRow("Bool: true") << QJsonValue(true) << QString() << false;
    QTest::newRow("Bool: false") << QJsonValue(false) << QString() << false;

    QTest::newRow("Double: 1.0") << QJsonValue(1.0) << QString() << false;

    QTest::newRow("Array") << QJsonValue(QJsonArray()) << QString() << false;
    QTest::newRow("Object") << QJsonValue(QJsonObject()) << QString() << false;
}

// Test: deserialize<QByteArray>() method ----------------------------------------------------------

void TestDeserialization::testDeserializeQByteArray()
{
    QFETCH(QJsonValue, input);
    QFETCH(QByteArray, expectedOutput);
    QFETCH(bool, expectedResult);

    QByteArray output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeQByteArray_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QByteArray>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    QTest::newRow("String: empty") << QJsonValue(QString()) << QByteArray() << true;
    QTest::newRow("String: a z A Z 0 9 + - ?")
            << QJsonValue("YSB6IEEgWiAwIDkgKyAtID8=") << QByteArray("a z A Z 0 9 + - ?") << true;

    // Negative tests
    QTest::newRow("Null") << QJsonValue() << QByteArray() << false;

    QTest::newRow("Bool: true") << QJsonValue(true) << QByteArray() << false;
    QTest::newRow("Bool: false") << QJsonValue(false) << QByteArray() << false;

    QTest::newRow("Double: 1.0") << QJsonValue(1.0) << QByteArray() << false;

#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
    QTest::newRow("String: invalid") << QJsonValue("xyz&!") << QByteArray() << false;
#endif

    QTest::newRow("Array") << QJsonValue(QJsonArray()) << QByteArray() << false;
    QTest::newRow("Object") << QJsonValue(QJsonObject()) << QByteArray() << false;
}

// Test: deserialize<QBitArray>() method -----------------------------------------------------------

void TestDeserialization::testDeserializeQBitArray()
{
    QFETCH(QJsonValue, input);
    QFETCH(QBitArray, expectedOutput);
    QFETCH(bool, expectedResult);

    QBitArray output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeQBitArray_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QBitArray>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    {
        const QJsonArray input;
        QTest::newRow("Array: empty") << QJsonValue(input) << QBitArray() << true;
    }

    {
        const QJsonArray input
        {
            1, 1, 1, 1, 0, 0, 0, 0,
            "0", "0", "1", "1", 1.0, 1.0F, 0.0, 0.0F
        };

        QBitArray expectedResult(16);
        expectedResult.setBit(0);
        expectedResult.setBit(1);
        expectedResult.setBit(2);
        expectedResult.setBit(3);

        expectedResult.clearBit(4);
        expectedResult.clearBit(5);
        expectedResult.clearBit(6);
        expectedResult.clearBit(7);

        expectedResult.clearBit(8);
        expectedResult.clearBit(9);
        expectedResult.setBit(10);
        expectedResult.setBit(11);

        expectedResult.setBit(12);
        expectedResult.setBit(13);
        expectedResult.clearBit(14);
        expectedResult.clearBit(15);

        QTest::newRow("Array: valid") << QJsonValue(input) << expectedResult << true;
    }

    // Negative tests
    QTest::newRow("Null") << QJsonValue() << QBitArray() << false;

    QTest::newRow("Bool: true") << QJsonValue(true) << QBitArray() << false;
    QTest::newRow("Bool: false") << QJsonValue(false) << QBitArray() << false;

    QTest::newRow("Double: 1.0") << QJsonValue(1.0) << QBitArray() << false;

    QTest::newRow("String: invalid") << QJsonValue("123") << QBitArray() << false;

    QTest::newRow("Array: invalid bit 1") << QJsonValue(QJsonArray({ 2 })) << QBitArray() << false;
    QTest::newRow("Array: invalid bit 2") << QJsonValue(QJsonArray({ -1 })) << QBitArray() << false;
    QTest::newRow("Array: invalid bit 3")
            << QJsonValue(QJsonArray({ 1.5 })) << QBitArray() << false;
    QTest::newRow("Array: invalid bit 4")
            << QJsonValue(QJsonArray({ -0.5 })) << QBitArray() << false;
    QTest::newRow("Array: invalid bit 5")
            << QJsonValue(QJsonArray({ false })) << QBitArray() << false;
    QTest::newRow("Array: invalid bit 6")
            << QJsonValue(QJsonArray({ true })) << QBitArray() << false;
    QTest::newRow("Array: invalid bit 7")
            << QJsonValue(QJsonArray({ "-1" })) << QBitArray() << false;
    QTest::newRow("Array: invalid bit 8")
            << QJsonValue(QJsonArray({ "2" })) << QBitArray() << false;

    QTest::newRow("Object") << QJsonValue(QJsonObject()) << QBitArray() << false;
}

// Test: deserialize<std::string>() method ---------------------------------------------------------

Q_DECLARE_METATYPE(std::string)

void TestDeserialization::testDeserializeStdString()
{
    QFETCH(QJsonValue, input);
    QFETCH(std::string, expectedOutput);
    QFETCH(bool, expectedResult);

    std::string output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeStdString_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<std::string>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    QTest::newRow("String: empty") << QJsonValue(QString()) << std::string() << true;
    QTest::newRow("String: a z A Z 0 9 + - ?")
            << QJsonValue("a z A Z 0 9 + - ?") << std::string("a z A Z 0 9 + - ?") << true;

    // Negative tests
    QTest::newRow("Null") << QJsonValue() << std::string() << false;

    QTest::newRow("Bool: true") << QJsonValue(true) << std::string() << false;
    QTest::newRow("Bool: false") << QJsonValue(false) << std::string() << false;

    QTest::newRow("Double: 1.0") << QJsonValue(1.0) << std::string() << false;

    QTest::newRow("Array") << QJsonValue(QJsonArray()) << std::string() << false;
    QTest::newRow("Object") << QJsonValue(QJsonObject()) << std::string() << false;
}

// Test: deserialize<std::wstring>() method --------------------------------------------------------

Q_DECLARE_METATYPE(std::wstring)

void TestDeserialization::testDeserializeStdWString()
{
    QFETCH(QJsonValue, input);
    QFETCH(std::wstring, expectedOutput);
    QFETCH(bool, expectedResult);

    std::wstring output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeStdWString_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<std::wstring>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    QTest::newRow("String: empty") << QJsonValue(QString()) << std::wstring() << true;
    QTest::newRow("String: a z A Z 0 9 + - ?")
            << QJsonValue("a z A Z 0 9 + - ?") << std::wstring(L"a z A Z 0 9 + - ?") << true;

    // Negative tests
    QTest::newRow("Null") << QJsonValue() << std::wstring() << false;

    QTest::newRow("Bool: true") << QJsonValue(true) << std::wstring() << false;
    QTest::newRow("Bool: false") << QJsonValue(false) << std::wstring() << false;

    QTest::newRow("Double: 1.0") << QJsonValue(1.0) << std::wstring() << false;

    QTest::newRow("Array") << QJsonValue(QJsonArray()) << std::wstring() << false;
    QTest::newRow("Object") << QJsonValue(QJsonObject()) << std::wstring() << false;
}

// Test: deserialize<std::u16string>() method ------------------------------------------------------

Q_DECLARE_METATYPE(std::u16string)

void TestDeserialization::testDeserializeStdU16String()
{
    QFETCH(QJsonValue, input);
    QFETCH(std::u16string, expectedOutput);
    QFETCH(bool, expectedResult);

    std::u16string output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeStdU16String_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<std::u16string>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    QTest::newRow("String: empty") << QJsonValue(QString()) << std::u16string() << true;
    QTest::newRow("String: a z A Z 0 9 + - ?")
            << QJsonValue("a z A Z 0 9 + - ?") << std::u16string(u"a z A Z 0 9 + - ?") << true;

    // Negative tests
    QTest::newRow("Null") << QJsonValue() << std::u16string() << false;

    QTest::newRow("Bool: true") << QJsonValue(true) << std::u16string() << false;
    QTest::newRow("Bool: false") << QJsonValue(false) << std::u16string() << false;

    QTest::newRow("Double: 1.0") << QJsonValue(1.0) << std::u16string() << false;

    QTest::newRow("Array") << QJsonValue(QJsonArray()) << std::u16string() << false;
    QTest::newRow("Object") << QJsonValue(QJsonObject()) << std::u16string() << false;
}

// Test: deserialize<std::u32string>() method ------------------------------------------------------

Q_DECLARE_METATYPE(std::u32string)

void TestDeserialization::testDeserializeStdU32String()
{
    QFETCH(QJsonValue, input);
    QFETCH(std::u32string, expectedOutput);
    QFETCH(bool, expectedResult);

    std::u32string output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeStdU32String_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<std::u32string>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    QTest::newRow("String: empty") << QJsonValue(QString()) << std::u32string() << true;
    QTest::newRow("String: a z A Z 0 9 + - ?")
            << QJsonValue("a z A Z 0 9 + - ?") << std::u32string(U"a z A Z 0 9 + - ?") << true;

    // Negative tests
    QTest::newRow("Null") << QJsonValue() << std::u32string() << false;

    QTest::newRow("Bool: true") << QJsonValue(true) << std::u32string() << false;
    QTest::newRow("Bool: false") << QJsonValue(false) << std::u32string() << false;

    QTest::newRow("Double: 1.0") << QJsonValue(1.0) << std::u32string() << false;

    QTest::newRow("Array") << QJsonValue(QJsonArray()) << std::u32string() << false;
    QTest::newRow("Object") << QJsonValue(QJsonObject()) << std::u32string() << false;
}

// Test: deserialize<QDate>() method ---------------------------------------------------------------

void TestDeserialization::testDeserializeQDate()
{
    QFETCH(QJsonValue, input);
    QFETCH(QDate, expectedOutput);
    QFETCH(bool, expectedResult);

    QDate output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeQDate_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QDate>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    QTest::newRow("String: empty") << QJsonValue(QString()) << QDate() << true;
    QTest::newRow("String: 2020-08-05") << QJsonValue("2020-08-05") << QDate(2020, 8, 5) << true;

    // Negative tests
    QTest::newRow("Null") << QJsonValue() << QDate() << false;

    QTest::newRow("Bool: true") << QJsonValue(true) << QDate() << false;
    QTest::newRow("Bool: false") << QJsonValue(false) << QDate() << false;

    QTest::newRow("Double: 1.0") << QJsonValue(1.0) << QDate() << false;

    QTest::newRow("String: invalid 1") << QJsonValue("asd") << QDate() << false;
    QTest::newRow("String: invalid 2") << QJsonValue("2020-13-05") << QDate() << false;

    QTest::newRow("Array") << QJsonValue(QJsonArray()) << QDate() << false;
    QTest::newRow("Object") << QJsonValue(QJsonObject()) << QDate() << false;
}

// Test: deserialize<QTime>() method ---------------------------------------------------------------

void TestDeserialization::testDeserializeQTime()
{
    QFETCH(QJsonValue, input);
    QFETCH(QTime, expectedOutput);
    QFETCH(bool, expectedResult);

    QTime output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeQTime_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QTime>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    QTest::newRow("String: empty") << QJsonValue(QString()) << QTime() << true;
    QTest::newRow("String: 12:34:56.789")
            << QJsonValue("12:34:56.789") << QTime(12, 34, 56, 789) << true;

    // Negative tests
    QTest::newRow("Null") << QJsonValue() << QTime() << false;

    QTest::newRow("Bool: true") << QJsonValue(true) << QTime() << false;
    QTest::newRow("Bool: false") << QJsonValue(false) << QTime() << false;

    QTest::newRow("Double: 1.0") << QJsonValue(1.0) << QTime() << false;

    QTest::newRow("String: invalid 1") << QJsonValue("asd") << QTime() << false;
    QTest::newRow("String: invalid 2") << QJsonValue("12:60:56.789") << QTime() << false;

    QTest::newRow("Array") << QJsonValue(QJsonArray()) << QTime() << false;
    QTest::newRow("Object") << QJsonValue(QJsonObject()) << QTime() << false;
}

// Test: deserialize<QDateTime>() method -----------------------------------------------------------

void TestDeserialization::testDeserializeQDateTime()
{
    QFETCH(QJsonValue, input);
    QFETCH(QDateTime, expectedOutput);
    QFETCH(bool, expectedResult);

    QDateTime output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeQDateTime_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QDateTime>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    QTest::newRow("String: empty") << QJsonValue(QString()) << QDateTime() << true;
    QTest::newRow("String: 2020-08-05T12:34:56.789Z")
            << QJsonValue("2020-08-05T12:34:56.789Z")
            << QDateTime(QDate(2020, 8, 5), QTime(12, 34, 56, 789), Qt::UTC)
            << true;

    // Negative tests
    QTest::newRow("Null") << QJsonValue() << QDateTime() << false;

    QTest::newRow("Bool: true") << QJsonValue(true) << QDateTime() << false;
    QTest::newRow("Bool: false") << QJsonValue(false) << QDateTime() << false;

    QTest::newRow("Double: 1.0") << QJsonValue(1.0) << QDateTime() << false;

    QTest::newRow("String: invalid 1") << QJsonValue(QString("asd")) << QDateTime() << false;
    QTest::newRow("String: invalid 2")
            << QJsonValue("2020138-05T12:34:56.789Z") << QDateTime() << false;

    QTest::newRow("Array") << QJsonValue(QJsonArray()) << QDateTime() << false;
    QTest::newRow("Object") << QJsonValue(QJsonObject()) << QDateTime() << false;
}

// Test: deserialize<QVariant>() method ------------------------------------------------------------

void TestDeserialization::testDeserializeQVariant()
{
    QFETCH(QJsonValue, input);
    QFETCH(QVariant, expectedOutput);
    QFETCH(bool, expectedResult);

    QVariant output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeQVariant_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QVariant>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    QTest::newRow("Null") << QJsonValue() << QVariant::fromValue(nullptr) << true;

    QTest::newRow("Bool: true") << QJsonValue(true) << QVariant(true) << true;
    QTest::newRow("Bool: false") << QJsonValue(false) << QVariant(false) << true;

    QTest::newRow("String: empty") << QJsonValue(QString()) << QVariant(QString()) << true;
    QTest::newRow("String: asd") << QJsonValue("asd") << QVariant(QString("asd")) << true;

    QTest::newRow("Double: 1.0") << QJsonValue(1.0) << QVariant(1.0) << true;
    QTest::newRow("Double: 0.0") << QJsonValue(0.0) << QVariant(0.0) << true;

    QTest::newRow("Array: empty") << QJsonValue(QJsonArray()) << QVariant(QVariantList()) << true;
    QTest::newRow("Array: non-empty")
            << QJsonValue(QJsonArray({ 1, 2, 3 }))
            << QVariant(QVariantList({ 1, 2, 3 }))
            << true;

    QTest::newRow("Object: empty") << QJsonValue(QJsonObject()) << QVariant(QVariantMap()) << true;
    QTest::newRow("Object: non-empty")
            << QJsonValue(QJsonObject({ {"a", 1}, {"b", 2} }))
            << QVariant(QVariantMap({ {"a", 1}, {"b", 2} }))
            << true;
}

// Test: deserialize<QUrl>() method ----------------------------------------------------------------

void TestDeserialization::testDeserializeQUrl()
{
    QFETCH(QJsonValue, input);
    QFETCH(QUrl, expectedOutput);
    QFETCH(bool, expectedResult);

    QUrl output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeQUrl_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QUrl>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    QTest::newRow("String: empty") << QJsonValue(QString()) << QUrl() << true;
    QTest::newRow("String: http://www.example.com")
            << QJsonValue("http://www.example.com") << QUrl("http://www.example.com") << true;

    // Negative tests
    QTest::newRow("Null") << QJsonValue() << QUrl() << false;

    QTest::newRow("Bool: true") << QJsonValue(true) << QUrl() << false;
    QTest::newRow("Bool: false") << QJsonValue(false) << QUrl() << false;

    QTest::newRow("Double: 1.0") << QJsonValue(1.0) << QUrl() << false;

    QTest::newRow("String: invalid 1") << QJsonValue("&http://www.example.com") << QUrl() << false;

    QTest::newRow("Array") << QJsonValue(QJsonArray()) << QUrl() << false;
    QTest::newRow("Object") << QJsonValue(QJsonObject()) << QUrl() << false;
}

// Test: deserialize<QUuid>() method ---------------------------------------------------------------

void TestDeserialization::testDeserializeQUuid()
{
    QFETCH(QJsonValue, input);
    QFETCH(QUuid, expectedOutput);
    QFETCH(bool, expectedResult);

    QUuid output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeQUuid_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QUuid>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    QTest::newRow("String: empty")
            << QJsonValue("{00000000-0000-0000-0000-000000000000}") << QUuid() << true;
    QTest::newRow("String: {01234567-89ab-cdef-0123-456789abcdef}")
            << QJsonValue("{01234567-89ab-cdef-0123-456789abcdef}")
            << QUuid("{01234567-89ab-cdef-0123-456789abcdef}")
            << true;

    // Negative tests
    QTest::newRow("Null") << QJsonValue() << QUuid() << false;

    QTest::newRow("Bool: true") << QJsonValue(true) << QUuid() << false;
    QTest::newRow("Bool: false") << QJsonValue(false) << QUuid() << false;

    QTest::newRow("Double: 1.0") << QJsonValue(1.0) << QUuid() << false;

    QTest::newRow("String: invalid 1") << QJsonValue("") << QUuid() << false;
    QTest::newRow("String: invalid 2") << QJsonValue("asd") << QUuid() << false;
    QTest::newRow("String: invalid 3")
            << QJsonValue("{x1234567-89ab-cdef-0123-456789abcdef}") << QUuid() << false;
    QTest::newRow("String: invalid 4")
            << QJsonValue("{1234567-89ab-cdef-0123-456789abcdef}") << QUuid() << false;

    QTest::newRow("Array") << QJsonValue(QJsonArray()) << QUuid() << false;
    QTest::newRow("Object") << QJsonValue(QJsonObject()) << QUuid() << false;
}

// Test: deserialize<QLocale>() method -------------------------------------------------------------

void TestDeserialization::testDeserializeQLocale()
{
    QFETCH(QJsonValue, input);
    QFETCH(QLocale, expectedOutput);
    QFETCH(bool, expectedResult);

    QLocale output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeQLocale_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QLocale>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    QTest::newRow("String: en-GB")
            << QJsonValue("en-GB")
            << QLocale(QLocale::English, QLocale::LatinScript, QLocale::UnitedKingdom)
            << true;
    QTest::newRow("String: de-AT")
            << QJsonValue("de-AT")
            << QLocale(QLocale::German, QLocale::Austria)
            << true;
    QTest::newRow("String: ru")
            << QJsonValue("ru")
            << QLocale(QLocale::Russian, QLocale::Russia)
            << true;
    QTest::newRow("String: empty") << QJsonValue("") << QLocale::c() << true;
    QTest::newRow("String: invalid") << QJsonValue("asd") << QLocale::c() << true;

    // Negative tests
    QTest::newRow("Null") << QJsonValue() << QLocale() << false;

    QTest::newRow("Bool: true") << QJsonValue(true) << QLocale() << false;
    QTest::newRow("Bool: false") << QJsonValue(false) << QLocale() << false;

    QTest::newRow("Double: 1.0") << QJsonValue(1.0) << QLocale() << false;

    QTest::newRow("Array") << QJsonValue(QJsonArray()) << QLocale() << false;
    QTest::newRow("Object") << QJsonValue(QJsonObject()) << QLocale() << false;
}

// Test: deserialize<QRegExp>() method -------------------------------------------------------------

void TestDeserialization::testDeserializeQRegExp()
{
    QFETCH(QJsonValue, input);
    QFETCH(QRegExp, expectedOutput);
    QFETCH(bool, expectedResult);

    QRegExp output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeQRegExp_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QRegExp>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    {
        const QJsonObject input
        {
            { "pattern",        "" },
            { "case_sensitive", true },
            { "syntax",         "RegExp" }
        };

        QTest::newRow("Object: default") << QJsonValue(input) << QRegExp() << true;
    }

    {
        const QJsonObject input
        {
            { "pattern",        "[a-z0-9]+" },
            { "case_sensitive", false },
            { "syntax",         "RegExp2" }
        };

        QTest::newRow("Object: with pattern")
                << QJsonValue(input)
                << QRegExp("[a-z0-9]+", Qt::CaseInsensitive, QRegExp::RegExp2)
                << true;
    }

    // Negative tests
    QTest::newRow("Null") << QJsonValue() << QRegExp() << false;

    QTest::newRow("Bool: true") << QJsonValue(true) << QRegExp() << false;
    QTest::newRow("Bool: false") << QJsonValue(false) << QRegExp() << false;

    QTest::newRow("Double: 1.0") << QJsonValue(1.0) << QRegExp() << false;

    QTest::newRow("String: invalid") << QJsonValue("asd") << QRegExp() << false;

    QTest::newRow("Array") << QJsonValue(QJsonArray()) << QRegExp() << false;

    QTest::newRow("Object: empty") << QJsonValue(QJsonObject()) << QRegExp() << false;

    {
        const QJsonObject input
        {
            { "case_sensitive", false },
            { "syntax",         "RegExp2" }
        };

        QTest::newRow("Object: missing param 1") << QJsonValue(input) << QRegExp() << false;
    }

    {
        const QJsonObject input
        {
            { "pattern",        "[a-z0-9]+" },
            { "syntax",         "RegExp2" }
        };

        QTest::newRow("Object: missing param 2") << QJsonValue(input) << QRegExp() << false;
    }

    {
        const QJsonObject input
        {
            { "pattern",        "[a-z0-9]+" },
            { "case_sensitive", false },
        };

        QTest::newRow("Object: missing param 3") << QJsonValue(input) << QRegExp() << false;
    }

    {
        const QJsonObject input
        {
            { "pattern",        0 },
            { "case_sensitive", false },
            { "syntax",         "RegExp2" }
        };

        QTest::newRow("Object: invalid param 1") << QJsonValue(input) << QRegExp() << false;
    }

    {
        const QJsonObject input
        {
            { "pattern",        "[a-z0-9]+" },
            { "case_sensitive", "asd" },
            { "syntax",         "RegExp2" }
        };

        QTest::newRow("Object: invalid param 2") << QJsonValue(input) << QRegExp() << false;
    }

    {
        const QJsonObject input
        {
            { "pattern",        "[a-z0-9]+" },
            { "case_sensitive", false },
            { "syntax",         0 }
        };

        QTest::newRow("Object: invalid param 3") << QJsonValue(input) << QRegExp() << false;
    }

    {
        const QJsonObject input
        {
            { "pattern",        "[a-z0-9]+" },
            { "case_sensitive", false },
            { "syntax",         "asd" }
        };

        QTest::newRow("Object: invalid param 4") << QJsonValue(input) << QRegExp() << false;
    }
}

// Test: deserialize<QRegularExpression>() method --------------------------------------------------

void TestDeserialization::testDeserializeQRegularExpression()
{
    QFETCH(QJsonValue, input);
    QFETCH(QRegularExpression, expectedOutput);
    QFETCH(bool, expectedResult);

    QRegularExpression output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeQRegularExpression_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QRegularExpression>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    {
        const QJsonObject input
        {
            { "pattern", "" },
            { "options", QJsonArray() }
        };

        QTest::newRow("Object: default") << QJsonValue(input) << QRegularExpression() << true;
    }

    {
        const QJsonObject input
        {
            { "pattern", "[a-z0-9]+" },
            { "options", QJsonArray { "DotMatchesEverything", "Multiline" } }
        };

        QTest::newRow("Object: with pattern")
                 << QJsonValue(input)
                 << QRegularExpression("[a-z0-9]+",
                                       QRegularExpression::DotMatchesEverythingOption |
                                       QRegularExpression::MultilineOption)
                 << true;
    }

    // Negative tests
    QTest::newRow("Null") << QJsonValue() << QRegularExpression() << false;

    QTest::newRow("Bool: true") << QJsonValue(true) << QRegularExpression() << false;
    QTest::newRow("Bool: false") << QJsonValue(false) << QRegularExpression() << false;

    QTest::newRow("Double: 1.0") << QJsonValue(1.0) << QRegularExpression() << false;

    QTest::newRow("String: invalid") << QJsonValue("asd") << QRegularExpression() << false;

    QTest::newRow("Array") << QJsonValue(QJsonArray()) << QRegularExpression() << false;

    QTest::newRow("Object: empty") << QJsonValue(QJsonObject()) << QRegularExpression() << false;

    {
        const QJsonObject input
        {
            { "options", QJsonArray({ "DotMatchesEverything", "Multiline" }) }
        };

        QTest::newRow("Object: missing param 1")
                << QJsonValue(input) << QRegularExpression() << false;
    }

    {
        const QJsonObject input
        {
            { "pattern", "[a-z0-9]+" }
        };

        QTest::newRow("Object: missing param 2")
                << QJsonValue(input) << QRegularExpression() << false;
    }

    {
        const QJsonObject input
        {
            { "pattern", 0 },
            { "options", QJsonArray({ "DotMatchesEverything", "Multiline" }) }
        };

        QTest::newRow("Object: invalid param 1")
                << QJsonValue(input) << QRegularExpression() << false;
    }

    {
        const QJsonObject input
        {
            { "pattern", "[a-z0-9]+" },
            { "options", 0 }
        };

        QTest::newRow("Object: invalid param 2")
                << QJsonValue(input) << QRegularExpression() << false;
    }

    {
        const QJsonObject input
        {
            { "pattern", "[a-z0-9]+" },
            { "options", QJsonArray({ "asd" }) }
        };

        QTest::newRow("Object: invalid param 3")
                << QJsonValue(input) << QRegularExpression() << false;
    }
}

// Test: deserialize<QSize>() method ---------------------------------------------------------------

void TestDeserialization::testDeserializeQSize()
{
    QFETCH(QJsonValue, input);
    QFETCH(QSize, expectedOutput);
    QFETCH(bool, expectedResult);

    QSize output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeQSize_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QSize>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    {
        const QJsonObject input
        {
            { "width",  -1 },
            { "height", -1 }
        };

        QTest::newRow("Object: default") << QJsonValue(input) << QSize() << true;
    }

    {
        const QJsonObject input
        {
            { "width",  123 },
            { "height", 456 }
        };

        QTest::newRow("Object: 123 x 456") << QJsonValue(input) << QSize(123, 456) << true;
    }

    // Negative tests
    QTest::newRow("Null") << QJsonValue() << QSize() << false;

    QTest::newRow("Bool: true") << QJsonValue(true) << QSize() << false;
    QTest::newRow("Bool: false") << QJsonValue(false) << QSize() << false;

    QTest::newRow("Double: 1.0") << QJsonValue(1.0) << QSize() << false;

    QTest::newRow("String: invalid") << QJsonValue("asd") << QSize() << false;

    QTest::newRow("Array") << QJsonValue(QJsonArray()) << QSize() << false;

    QTest::newRow("Object: empty") << QJsonValue(QJsonObject()) << QSize() << false;

    {
        const QJsonObject input
        {
            { "width",  123 }
        };

        QTest::newRow("Object: missing param 1") << QJsonValue(input) << QSize() << false;
    }

    {
        const QJsonObject input
        {
            { "height", 456 }
        };

        QTest::newRow("Object: missing param 2") << QJsonValue(input) << QSize() << false;
    }

    {
        const QJsonObject input
        {
            { "width",  "asd" },
            { "height", 456 }
        };

        QTest::newRow("Object: invalid param 1") << QJsonValue(input) << QSize() << false;
    }

    {
        const QJsonObject input
        {
            { "width",  123 },
            { "height", "asd" }
        };

        QTest::newRow("Object: invalid param 2") << QJsonValue(input) << QSize() << false;
    }
}

// Test: deserialize<QSizeF>() method --------------------------------------------------------------

void TestDeserialization::testDeserializeQSizeF()
{
    QFETCH(QJsonValue, input);
    QFETCH(QSizeF, expectedOutput);
    QFETCH(bool, expectedResult);

    QSizeF output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeQSizeF_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QSizeF>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    {
        const QJsonObject input
        {
            { "width",  -1.0 },
            { "height", -1.0 }
        };

        QTest::newRow("Object: default") << QJsonValue(input) << QSizeF() << true;
    }

    {
        const QJsonObject input
        {
            { "width",  123.4 },
            { "height", 567.8 }
        };

        QTest::newRow("Object: 123.4 x 567.8")
                << QJsonValue(input)  << QSizeF(123.4, 567.8) << true;
    }

    // Negative tests
    QTest::newRow("Null") << QJsonValue() << QSizeF() << false;

    QTest::newRow("Bool: true") << QJsonValue(true) << QSizeF() << false;
    QTest::newRow("Bool: false") << QJsonValue(false) << QSizeF() << false;

    QTest::newRow("Double: 1.0") << QJsonValue(1.0) << QSizeF() << false;

    QTest::newRow("String: invalid") << QJsonValue("asd") << QSizeF() << false;

    QTest::newRow("Array") << QJsonValue(QJsonArray()) << QSizeF() << false;

    QTest::newRow("Object: empty") << QJsonValue(QJsonObject()) << QSizeF() << false;

    {
        const QJsonObject input
        {
            { "width",  123.4 }
        };

        QTest::newRow("Object: missing param 1") << QJsonValue(input) << QSizeF() << false;
    }

    {
        const QJsonObject input
        {
            { "height", 456.7 }
        };

        QTest::newRow("Object: missing param 2") << QJsonValue(input) << QSizeF() << false;
    }

    {
        const QJsonObject input
        {
            { "width",  "asd" },
            { "height", 456.7 }
        };

        QTest::newRow("Object: invalid param 1") << QJsonValue(input) << QSizeF() << false;
    }

    {
        const QJsonObject input
        {
            { "width",  123.4 },
            { "height", "asd" }
        };

        QTest::newRow("Object: invalid param 2") << QJsonValue(input) << QSizeF() << false;
    }
}

// Test: deserialize<QPoint>() method --------------------------------------------------------------

void TestDeserialization::testDeserializeQPoint()
{
    QFETCH(QJsonValue, input);
    QFETCH(QPoint, expectedOutput);
    QFETCH(bool, expectedResult);

    QPoint output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeQPoint_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QPoint>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    {
        const QJsonObject input
        {
            { "x", 0 },
            { "y", 0 }
        };

        QTest::newRow("Object: default") << QJsonValue(input) << QPoint() << true;
    }

    {
        const QJsonObject input
        {
            { "x", 123 },
            { "y", 456 }
        };

        QTest::newRow("Object: 123 x 456") << QJsonValue(input) << QPoint(123, 456) << true;
    }

    // Negative tests
    QTest::newRow("Null") << QJsonValue() << QPoint() << false;

    QTest::newRow("Bool: true") << QJsonValue(true) << QPoint() << false;
    QTest::newRow("Bool: false") << QJsonValue(false) << QPoint() << false;

    QTest::newRow("Double: 1.0") << QJsonValue(1.0) << QPoint() << false;

    QTest::newRow("String: invalid") << QJsonValue("asd") << QPoint() << false;

    QTest::newRow("Array") << QJsonValue(QJsonArray()) << QPoint() << false;

    QTest::newRow("Object: empty") << QJsonValue(QJsonObject()) << QPoint() << false;

    {
        const QJsonObject input
        {
            { "x", 123 }
        };

        QTest::newRow("Object: missing param 1") << QJsonValue(input) << QPoint() << false;
    }

    {
        const QJsonObject input
        {
            { "y", 456 }
        };

        QTest::newRow("Object: missing param 2") << QJsonValue(input) << QPoint() << false;
    }

    {
        const QJsonObject input
        {
            { "x", "asd" },
            { "y", 456 }
        };

        QTest::newRow("Object: invalid param 1") << QJsonValue(input) << QPoint() << false;
    }

    {
        const QJsonObject input
        {
            { "x", 123 },
            { "y", "asd" }
        };

        QTest::newRow("Object: invalid param 2") << QJsonValue(input) << QPoint() << false;
    }
}

// Test: deserialize<QPointF>() method -------------------------------------------------------------

void TestDeserialization::testDeserializeQPointF()
{
    QFETCH(QJsonValue, input);
    QFETCH(QPointF, expectedOutput);
    QFETCH(bool, expectedResult);

    QPointF output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeQPointF_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QPointF>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    {
        const QJsonObject input
        {
            { "x", 0.0 },
            { "y", 0.0 }
        };

        QTest::newRow("Object: default") << QJsonValue(input) << QPointF() << true;
    }

    {
        const QJsonObject input
        {
            { "x", 123.4 },
            { "y", 567.8 }
        };

        QTest::newRow("Object: 123 x 456") << QJsonValue(input) << QPointF(123.4, 567.8) << true;
    }

    // Negative tests
    QTest::newRow("Null") << QJsonValue() << QPointF() << false;

    QTest::newRow("Bool: true") << QJsonValue(true) << QPointF() << false;
    QTest::newRow("Bool: false") << QJsonValue(false) << QPointF() << false;

    QTest::newRow("Double: 1.0") << QJsonValue(1.0) << QPointF() << false;

    QTest::newRow("String: invalid") << QJsonValue("asd") << QPointF() << false;

    QTest::newRow("Array") << QJsonValue(QJsonArray()) << QPointF() << false;

    QTest::newRow("Object: empty") << QJsonValue(QJsonObject()) << QPointF() << false;

    {
        const QJsonObject input
        {
            { "x", 123.4 }
        };

        QTest::newRow("Object: missing param 1") << QJsonValue(input) << QPointF() << false;
    }

    {
        const QJsonObject input
        {
            { "y", 567.8 }
        };

        QTest::newRow("Object: missing param 2") << QJsonValue(input) << QPointF() << false;
    }

    {
        const QJsonObject input
        {
            { "x", "asd" },
            { "y", 567.8 }
        };

        QTest::newRow("Object: invalid param 1") << QJsonValue(input) << QPointF() << false;
    }

    {
        const QJsonObject input
        {
            { "x", 123.4 },
            { "y", "asd" }
        };

        QTest::newRow("Object: invalid param 2") << QJsonValue(input) << QPointF() << false;
    }
}

// Test: deserialize<QLine>() method ---------------------------------------------------------------

void TestDeserialization::testDeserializeQLine()
{
    QFETCH(QJsonValue, input);
    QFETCH(QLine, expectedOutput);
    QFETCH(bool, expectedResult);

    QLine output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeQLine_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QLine>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    {
        const QJsonObject input
        {
            { "x1", 0 },
            { "y1", 0 },
            { "x2", 0 },
            { "y2", 0 }
        };

        QTest::newRow("Object: default") << QJsonValue(input) << QLine() << true;
    }

    {
        const QJsonObject input
        {
            { "x1", 123 },
            { "y1", 456 },
            { "x2", 777 },
            { "y2", 888 }
        };

        QTest::newRow("Object: 123 x 456, 777 x 888")
                << QJsonValue(input) << QLine(123, 456, 777, 888) << true;
    }

    // Negative tests
    QTest::newRow("Null") << QJsonValue() << QLine() << false;

    QTest::newRow("Bool: true") << QJsonValue(true) << QLine() << false;
    QTest::newRow("Bool: false") << QJsonValue(false) << QLine() << false;

    QTest::newRow("Double: 1.0") << QJsonValue(1.0) << QLine() << false;

    QTest::newRow("String: invalid") << QJsonValue("asd") << QLine() << false;

    QTest::newRow("Array") << QJsonValue(QJsonArray()) << QLine() << false;

    QTest::newRow("Object: empty") << QJsonValue(QJsonObject()) << QLine() << false;

    {
        const QJsonObject input
        {
            { "y1", 456 },
            { "x2", 777 },
            { "y2", 888 }
        };

        QTest::newRow("Object: missing param 1") << QJsonValue(input) << QLine() << false;
    }

    {
        const QJsonObject input
        {
            { "x1", 123 },
            { "x2", 777 },
            { "y2", 888 }
        };

        QTest::newRow("Object: missing param 2") << QJsonValue(input) << QLine() << false;
    }

    {
        const QJsonObject input
        {
            { "x1", 123 },
            { "y1", 456 },
            { "y2", 888 }
        };

        QTest::newRow("Object: missing param 3") << QJsonValue(input) << QLine() << false;
    }

    {
        const QJsonObject input
        {
            { "x1", 123 },
            { "y1", 456 },
            { "x2", 777 }
        };

        QTest::newRow("Object: missing param 4") << QJsonValue(input) << QLine() << false;
    }

    {
        const QJsonObject input
        {
            { "x1", "asd" },
            { "y1", 456 },
            { "x2", 777 },
            { "y2", 888 }
        };

        QTest::newRow("Object: invalid param 1") << QJsonValue(input) << QLine() << false;
    }

    {
        const QJsonObject input
        {
            { "x1", 123 },
            { "y1", "asd" },
            { "x2", 777 },
            { "y2", 888 }
        };

        QTest::newRow("Object: invalid param 2") << QJsonValue(input) << QLine() << false;
    }

    {
        const QJsonObject input
        {
            { "x1", 123 },
            { "y1", 456 },
            { "x2", "asd" },
            { "y2", 888 }
        };

        QTest::newRow("Object: invalid param 3") << QJsonValue(input) << QLine() << false;
    }

    {
        const QJsonObject input
        {
            { "x1", 123 },
            { "y1", 456 },
            { "x2", 777 },
            { "y2", "asd" }
        };

        QTest::newRow("Object: invalid param 4") << QJsonValue(input) << QLine() << false;
    }
}

// Test: deserialize<QLineF>() method --------------------------------------------------------------

void TestDeserialization::testDeserializeQLineF()
{
    QFETCH(QJsonValue, input);
    QFETCH(QLineF, expectedOutput);
    QFETCH(bool, expectedResult);

    QLineF output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeQLineF_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QLineF>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    {
        const QJsonObject input
        {
            { "x1", 0.0 },
            { "y1", 0.0 },
            { "x2", 0.0 },
            { "y2", 0.0 }
        };

        QTest::newRow("Object: default") << QJsonValue(input) << QLineF() << true;
    }

    {
        const QJsonObject input
        {
            { "x1", 123.4 },
            { "y1", 567.8 },
            { "x2", 111.1 },
            { "y2", 222.2 }
        };

        QTest::newRow("Object: 123.4 x 567.8, 111.1 x 222.2")
                << QJsonValue(input) << QLineF(123.4, 567.8, 111.1, 222.2) << true;
    }

    // Negative tests
    QTest::newRow("Null") << QJsonValue() << QLineF() << false;

    QTest::newRow("Bool: true") << QJsonValue(true) << QLineF() << false;
    QTest::newRow("Bool: false") << QJsonValue(false) << QLineF() << false;

    QTest::newRow("Double: 1.0") << QJsonValue(1.0) << QLineF() << false;

    QTest::newRow("String: invalid") << QJsonValue("asd") << QLineF() << false;

    QTest::newRow("Array") << QJsonValue(QJsonArray()) << QLineF() << false;

    QTest::newRow("Object: empty") << QJsonValue(QJsonObject()) << QLineF() << false;

    {
        const QJsonObject input
        {
            { "y1", 0.0 },
            { "x2", 0.0 },
            { "y2", 0.0 }
        };

        QTest::newRow("Object: missing param 1") << QJsonValue(input) << QLineF() << false;
    }

    {
        const QJsonObject input
        {
            { "x1", 0.0 },
            { "x2", 0.0 },
            { "y2", 0.0 }
        };

        QTest::newRow("Object: missing param 2") << QJsonValue(input) << QLineF() << false;
    }

    {
        const QJsonObject input
        {
            { "x1", 0.0 },
            { "y1", 0.0 },
            { "y2", 0.0 }
        };

        QTest::newRow("Object: missing param 3") << QJsonValue(input) << QLineF() << false;
    }

    {
        const QJsonObject input
        {
            { "x1", 0.0 },
            { "y1", 0.0 },
            { "x2", 0.0 }
        };

        QTest::newRow("Object: missing param 4") << QJsonValue(input) << QLineF() << false;
    }

    {
        const QJsonObject input
        {
            { "x1", "asd" },
            { "y1", 0.0 },
            { "x2", 0.0 },
            { "y2", 0.0 }
        };

        QTest::newRow("Object: missing param 1") << QJsonValue(input) << QLineF() << false;
    }

    {
        const QJsonObject input
        {
            { "x1", 0.0 },
            { "y1", "asd" },
            { "x2", 0.0 },
            { "y2", 0.0 }
        };

        QTest::newRow("Object: missing param 2") << QJsonValue(input) << QLineF() << false;
    }

    {
        const QJsonObject input
        {
            { "x1", 0.0 },
            { "y1", 0.0 },
            { "x2", "asd" },
            { "y2", 0.0 }
        };

        QTest::newRow("Object: missing param 3") << QJsonValue(input) << QLineF() << false;
    }

    {
        const QJsonObject input
        {
            { "x1", 0.0 },
            { "y1", 0.0 },
            { "x2", 0.0 },
            { "y2", "asd" }
        };

        QTest::newRow("Object: missing param 4") << QJsonValue(input) << QLineF() << false;
    }
}

// Test: deserialize<QRect>() method ---------------------------------------------------------------

void TestDeserialization::testDeserializeQRect()
{
    QFETCH(QJsonValue, input);
    QFETCH(QRect, expectedOutput);
    QFETCH(bool, expectedResult);

    QRect output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeQRect_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QRect>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    {
        const QJsonObject input
        {
            { "x",      0 },
            { "y",      0 },
            { "width",  0 },
            { "height", 0 }
        };

        QTest::newRow("Object: default") << QJsonValue(input) << QRect() << true;
    }

    {
        const QJsonObject input
        {
            { "x",      123 },
            { "y",      456 },
            { "width",  777 },
            { "height", 888 }
        };

        QTest::newRow("Object: 123 x 456, 777 x 888")
                << QJsonValue(input) << QRect(123, 456, 777, 888) << true;
    }

    // Negative tests
    QTest::newRow("Null") << QJsonValue() << QRect() << false;

    QTest::newRow("Bool: true") << QJsonValue(true) << QRect() << false;
    QTest::newRow("Bool: false") << QJsonValue(false) << QRect() << false;

    QTest::newRow("Double: 1.0") << QJsonValue(1.0) << QRect() << false;

    QTest::newRow("String: invalid") << QJsonValue("asd") << QRect() << false;

    QTest::newRow("Array") << QJsonValue(QJsonArray()) << QRect() << false;

    QTest::newRow("Object: empty") << QJsonValue(QJsonObject()) << QRect() << false;

    {
        const QJsonObject input
        {
            { "y",      456 },
            { "width",  777 },
            { "height", 888 }
        };

        QTest::newRow("Object: missing param 1") << QJsonValue(input) << QRect() << false;
    }

    {
        const QJsonObject input
        {
            { "x",      123 },
            { "width",  777 },
            { "height", 888 }
        };

        QTest::newRow("Object: missing param 2") << QJsonValue(input) << QRect() << false;
    }

    {
        const QJsonObject input
        {
            { "x",      123 },
            { "y",      456 },
            { "height", 888 }
        };

        QTest::newRow("Object: missing param 3") << QJsonValue(input) << QRect() << false;
    }

    {
        const QJsonObject input
        {
            { "x",      123 },
            { "y",      456 },
            { "width",  777 }
        };

        QTest::newRow("Object: missing param 4") << QJsonValue(input) << QRect() << false;
    }

    {
        const QJsonObject input
        {
            { "x",      "asd" },
            { "y",      456 },
            { "width",  777 },
            { "height", 888 }
        };

        QTest::newRow("Object: invalid param 1") << QJsonValue(input) << QRect() << false;
    }

    {
        const QJsonObject input
        {
            { "x",      123 },
            { "y",      "asd" },
            { "width",  777 },
            { "height", 888 }
        };

        QTest::newRow("Object: invalid param 2") << QJsonValue(input) << QRect() << false;
    }

    {
        const QJsonObject input
        {
            { "x",      123 },
            { "y",      456 },
            { "width",  "asd" },
            { "height", 888 }
        };

        QTest::newRow("Object: invalid param 3") << QJsonValue(input) << QRect() << false;
    }

    {
        const QJsonObject input
        {
            { "x",      123 },
            { "y",      456 },
            { "width",  777 },
            { "height", "asd" }
        };

        QTest::newRow("Object: invalid param 4") << QJsonValue(input) << QRect() << false;
    }
}

// Test: deserialize<QRectF>() method --------------------------------------------------------------

void TestDeserialization::testDeserializeQRectF()
{
    QFETCH(QJsonValue, input);
    QFETCH(QRectF, expectedOutput);
    QFETCH(bool, expectedResult);

    QRectF output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeQRectF_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QRectF>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    {
        const QJsonObject input
        {
            { "x",      0.0 },
            { "y",      0.0 },
            { "width",  0.0 },
            { "height", 0.0 }
        };

        QTest::newRow("Object: default") << QJsonValue(input) << QRectF() << true;
    }

    {
        const QJsonObject input
        {
            { "x",      123.4 },
            { "y",      567.8 },
            { "width",  111.1 },
            { "height", 222.2 }
        };

        QTest::newRow("Object: 123.4 x 567.8, 111.1 x 222.2")
                << QJsonValue(input) << QRectF(123.4, 567.8, 111.1, 222.2) << true;
    }

    // Negative tests
    QTest::newRow("Null") << QJsonValue() << QRectF() << false;

    QTest::newRow("Bool: true") << QJsonValue(true) << QRectF() << false;
    QTest::newRow("Bool: false") << QJsonValue(false) << QRectF() << false;

    QTest::newRow("Double: 1.0") << QJsonValue(1.0) << QRectF() << false;

    QTest::newRow("String: invalid") << QJsonValue("asd") << QRectF() << false;

    QTest::newRow("Array") << QJsonValue(QJsonArray()) << QRectF() << false;

    QTest::newRow("Object: empty") << QJsonValue(QJsonObject()) << QRectF() << false;

    {
        const QJsonObject input
        {
            { "y",      567.8 },
            { "width",  111.1 },
            { "height", 222.2 }
        };

        QTest::newRow("Object: missing param 1") << QJsonValue(input) << QRectF() << false;
    }

    {
        const QJsonObject input
        {
            { "x",      123.4 },
            { "width",  111.1 },
            { "height", 222.2 }
        };

        QTest::newRow("Object: missing param 2") << QJsonValue(input) << QRectF() << false;
    }

    {
        const QJsonObject input
        {
            { "x",      123.4 },
            { "y",      567.8 },
            { "height", 222.2 }
        };

        QTest::newRow("Object: missing param 3") << QJsonValue(input) << QRectF() << false;
    }

    {
        const QJsonObject input
        {
            { "x",      123.4 },
            { "y",      567.8 },
            { "width",  111.1 }
        };

        QTest::newRow("Object: missing param 4") << QJsonValue(input) << QRectF() << false;
    }

    {
        const QJsonObject input
        {
            { "x",      "asd" },
            { "y",      567.8 },
            { "width",  111.1 },
            { "height", 222.2 }
        };

        QTest::newRow("Object: invalid param 1") << QJsonValue(input) << QRectF() << false;
    }

    {
        const QJsonObject input
        {
            { "x",      123.4 },
            { "y",      "asd" },
            { "width",  111.1 },
            { "height", 222.2 }
        };

        QTest::newRow("Object: invalid param 2") << QJsonValue(input) << QRectF() << false;
    }

    {
        const QJsonObject input
        {
            { "x",      123.4 },
            { "y",      567.8 },
            { "width",  "asd" },
            { "height", 222.2 }
        };

        QTest::newRow("Object: invalid param 3") << QJsonValue(input) << QRectF() << false;
    }

    {
        const QJsonObject input
        {
            { "x",      123.4 },
            { "y",      567.8 },
            { "width",  111.1 },
            { "height", "asd" }
        };

        QTest::newRow("Object: invalid param 4") << QJsonValue(input) << QRectF() << false;
    }
}

// Test: deserialize<QStringList>() method ---------------------------------------------------------

void TestDeserialization::testDeserializeQStringList()
{
    QFETCH(QJsonValue, input);
    QFETCH(QStringList, expectedOutput);
    QFETCH(bool, expectedResult);

    QStringList output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeQStringList_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QStringList>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    QTest::newRow("Array: empty") << QJsonValue(QJsonArray()) << QStringList() << true;

    {
        const QStringList expectedOutput { "a", "b", "C", "D", "1" };

        QTest::newRow("Array: non-empty")
                << QJsonValue(QJsonArray::fromStringList(expectedOutput)) << expectedOutput << true;
    }

    // Negative tests
    QTest::newRow("Null") << QJsonValue() << QStringList() << false;

    QTest::newRow("Bool: true") << QJsonValue(true) << QStringList() << false;
    QTest::newRow("Bool: false") << QJsonValue(false) << QStringList() << false;

    QTest::newRow("Double: 1.0") << QJsonValue(1.0) << QStringList() << false;

    QTest::newRow("String") << QJsonValue("asd") << QStringList() << false;

    QTest::newRow("Object") << QJsonValue(QJsonObject()) << QStringList() << false;

    QTest::newRow("Array: invalid") << QJsonValue(QJsonArray {0}) << QStringList() << false;
}

// Test: deserialize<QJsonValue>() method ----------------------------------------------------------

void TestDeserialization::testDeserializeQJsonValue()
{
    QFETCH(QJsonValue, input);
    QFETCH(QJsonValue, expectedOutput);
    QFETCH(bool, expectedResult);

    QJsonValue output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeQJsonValue_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QJsonValue>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    QTest::newRow("Null")   << QJsonValue()         << QJsonValue()         << true;
    QTest::newRow("Bool")   << QJsonValue(true)     << QJsonValue(true)     << true;
    QTest::newRow("Double") << QJsonValue(123.4)    << QJsonValue(123.4)    << true;
    QTest::newRow("String") << QJsonValue("abc123") << QJsonValue("abc123") << true;

    {
        const QJsonArray value
        {
            QJsonValue(), false, 123.4
        };
        QTest::newRow("Array") << QJsonValue(value) << QJsonValue(value) << true;
    }

    {
        const QJsonObject value
        {
            { "null",   QJsonValue() },
            { "bool",   false },
            { "double", 123.4 }
        };
        QTest::newRow("Object") << QJsonValue(value) << QJsonValue(value) << true;
    }

    // Negative tests
}

// Test: deserialize<QJsonArray>() method ----------------------------------------------------------

void TestDeserialization::testDeserializeQJsonArray()
{
    QFETCH(QJsonValue, input);
    QFETCH(QJsonArray, expectedOutput);
    QFETCH(bool, expectedResult);

    QJsonArray output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeQJsonArray_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QJsonArray>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    QTest::newRow("empty") << QJsonValue(QJsonArray()) << QJsonArray() << true;

    {
        const QJsonArray value
        {
            QJsonValue(), false, 123.4
        };
        QTest::newRow("non-empty") << QJsonValue(value) << value << true;
    }

    // Negative tests
    QTest::newRow("Null")   << QJsonValue()              << QJsonArray() << false;
    QTest::newRow("Bool")   << QJsonValue(true)          << QJsonArray() << false;
    QTest::newRow("Double") << QJsonValue(123.4)         << QJsonArray() << false;
    QTest::newRow("String") << QJsonValue("abc123")      << QJsonArray() << false;
    QTest::newRow("Object") << QJsonValue(QJsonObject()) << QJsonArray() << false;
}

// Test: deserialize<QJsonObject>() method ---------------------------------------------------------

void TestDeserialization::testDeserializeQJsonObject()
{
    QFETCH(QJsonValue, input);
    QFETCH(QJsonObject, expectedOutput);
    QFETCH(bool, expectedResult);

    QJsonObject output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeQJsonObject_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QJsonObject>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    QTest::newRow("empty") << QJsonValue(QJsonObject()) << QJsonObject() << true;

    {
        const QJsonObject value
        {
            { "null",   QJsonValue() },
            { "bool",   false },
            { "double", 123.4 }
        };
        QTest::newRow("non-empty") << QJsonValue(value) << value << true;
    }

    // Negative tests
    QTest::newRow("Null")   << QJsonValue()             << QJsonObject() << false;
    QTest::newRow("Bool")   << QJsonValue(true)         << QJsonObject() << false;
    QTest::newRow("Double") << QJsonValue(123.4)        << QJsonObject() << false;
    QTest::newRow("String") << QJsonValue("abc123")     << QJsonObject() << false;
    QTest::newRow("Array")  << QJsonValue(QJsonArray()) << QJsonObject() << false;
}

// Test: deserialize<QJsonDocument>() method -------------------------------------------------------

void TestDeserialization::testDeserializeQJsonDocument()
{
    QFETCH(QJsonValue, input);
    QFETCH(QJsonDocument, expectedOutput);
    QFETCH(bool, expectedResult);

    QJsonDocument output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeQJsonDocument_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QJsonDocument>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    QTest::newRow("Null") << QJsonValue() << QJsonDocument() << true;

    {
        const QJsonArray value
        {
            QJsonValue(), false, 123.4
        };
        QTest::newRow("array") << QJsonValue(value) << QJsonDocument(value) << true;
    }

    {
        const QJsonObject value
        {
            { "null",   QJsonValue() },
            { "bool",   false },
            { "double", 123.4 }
        };
        QTest::newRow("object") << QJsonValue(value) << QJsonDocument(value) << true;
    }

    // Negative tests
    QTest::newRow("Bool")   << QJsonValue(true)     << QJsonDocument() << false;
    QTest::newRow("Double") << QJsonValue(123.4)    << QJsonDocument() << false;
    QTest::newRow("String") << QJsonValue("abc123") << QJsonDocument() << false;
}

// Test: deserialize<QCborValue>() method ----------------------------------------------------------

#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
void TestDeserialization::testDeserializeQCborValue()
{
    QFETCH(QJsonValue, input);
    QFETCH(QCborValue, expectedOutput);
    QFETCH(bool, expectedResult);

    QCborValue output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output.type(), expectedOutput.type());

        if (output.type() == QCborValue::Map)
        {
            const auto outputMap         = output.toMap();
            const auto expectedOutputMap = expectedOutput.toMap();
            QCOMPARE(outputMap.size(), expectedOutputMap.size());

            for (auto it = outputMap.cbegin(); it != outputMap.cend(); it++)
            {
                QCOMPARE(it.value(), expectedOutputMap.value(it.key()));
            }
        }
        else
        {
            QCOMPARE(output, expectedOutput);
        }
    }
}

void TestDeserialization::testDeserializeQCborValue_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QCborValue>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    QTest::newRow("null")   << QJsonValue()         << QCborValue(nullptr)  << true;
    QTest::newRow("bool")   << QJsonValue(true)     << QCborValue(true)     << true;
    QTest::newRow("int")    << QJsonValue(100)      << QCborValue(100)      << true;
    QTest::newRow("double") << QJsonValue(123.4)    << QCborValue(123.4)    << true;
    QTest::newRow("string") << QJsonValue("abc123") << QCborValue("abc123") << true;

    {
        const QCborArray value
        {
            QCborValue(nullptr), false, 123.4
        };
        QTest::newRow("array") << QJsonValue(value.toJsonArray()) << QCborValue(value) << true;
    }

    {
        const QCborMap value
        {
            { "null",   QCborValue(nullptr) },
            { "bool",   false },
            { "double", 123.4 }
        };
        QTest::newRow("object") << QJsonValue(value.toJsonObject()) << QCborValue(value) << true;
    }

    // Negative tests
}
#endif

// Test: deserialize<QCborArray>() method ----------------------------------------------------------

#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
void TestDeserialization::testDeserializeQCborArray()
{
    QFETCH(QJsonValue, input);
    QFETCH(QCborArray, expectedOutput);
    QFETCH(bool, expectedResult);

    QCborArray output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeQCborArray_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QCborArray>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    QTest::newRow("array: empty") << QJsonValue(QJsonArray()) << QCborArray() << true;

    {
        const QCborArray value
        {
            QCborValue(nullptr), false, 123.4
        };
        QTest::newRow("array: non-empty") << QJsonValue(value.toJsonArray()) << value << true;
    }

    // Negative tests
    QTest::newRow("null")   << QJsonValue()              << QCborArray() << false;
    QTest::newRow("bool")   << QJsonValue(true)          << QCborArray() << false;
    QTest::newRow("double") << QJsonValue(123.4)         << QCborArray() << false;
    QTest::newRow("string") << QJsonValue("abc123")      << QCborArray() << false;
    QTest::newRow("object") << QJsonValue(QJsonObject()) << QCborArray() << false;
}
#endif

// Test: deserialize<QCborMap>() method ------------------------------------------------------------

#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
void TestDeserialization::testDeserializeQCborMap()
{
    QFETCH(QJsonValue, input);
    QFETCH(QCborMap, expectedOutput);
    QFETCH(bool, expectedResult);

    QCborMap output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output.size(), expectedOutput.size());

        for (auto it = output.cbegin(); it != output.cend(); it++)
        {
            QCOMPARE(it.value(), expectedOutput.value(it.key()));
        }
    }
}

void TestDeserialization::testDeserializeQCborMap_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QCborMap>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    QTest::newRow("object: empty") << QJsonValue(QJsonObject()) << QCborMap() << true;

    {
        const QCborMap value
        {
            { "null",   QCborValue(nullptr) },
            { "bool",   false },
            { "double", 123.4 }
        };
        QTest::newRow("object: non-empty") << QJsonValue(value.toJsonObject()) << value << true;
    }

    // Negative tests
    QTest::newRow("null")   << QJsonValue()             << QCborMap() << false;
    QTest::newRow("bool")   << QJsonValue(true)         << QCborMap() << false;
    QTest::newRow("double") << QJsonValue(123.4)        << QCborMap() << false;
    QTest::newRow("string") << QJsonValue("abc123")     << QCborMap() << false;
    QTest::newRow("array")  << QJsonValue(QJsonArray()) << QCborMap() << false;
}
#endif

// Test: deserialize<QCborSimpleType>() method -----------------------------------------------------

#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
void TestDeserialization::testDeserializeQCborSimpleType()
{
    QFETCH(QJsonValue, input);
    QFETCH(QCborSimpleType, expectedOutput);
    QFETCH(bool, expectedResult);

    QCborSimpleType output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeQCborSimpleType_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QCborSimpleType>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    QTest::newRow("null")        << QJsonValue()      << QCborSimpleType::Null  << true;
    QTest::newRow("bool: true")  << QJsonValue(true)  << QCborSimpleType::True  << true;
    QTest::newRow("bool: false") << QJsonValue(false) << QCborSimpleType::False << true;
    QTest::newRow("undefined")
            << QJsonValue(QJsonValue::Undefined) << QCborSimpleType::Undefined << true;

    // Negative tests
    QTest::newRow("double") << QJsonValue(123.4)         << QCborSimpleType() << false;
    QTest::newRow("string") << QJsonValue("abc123")      << QCborSimpleType() << false;
    QTest::newRow("array")  << QJsonValue(QJsonArray())  << QCborSimpleType() << false;
    QTest::newRow("object") << QJsonValue(QJsonObject()) << QCborSimpleType() << false;
}
#endif

// Test: deserialize<QPair>() method ---------------------------------------------------------------

using QPairStringInt = QPair<QString, int>;

void TestDeserialization::testDeserializeQPair()
{
    QFETCH(QJsonValue, input);
    QFETCH(QPairStringInt, expectedOutput);
    QFETCH(bool, expectedResult);

    QPairStringInt output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeQPair_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QPairStringInt>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    {
        const QJsonObject value
        {
            { "first",  "asd" },
            { "second", 123 }
        };
        QTest::newRow("object: 1") << QJsonValue(value) << QPairStringInt("asd", 123) << true;
    }

    {
        const QJsonObject value
        {
            { "first",  "abc" },
            { "second", 456 }
        };
        QTest::newRow("object: 2") << QJsonValue(value) << QPairStringInt("abc", 456) << true;
    }

    // Negative tests
    QTest::newRow("null")          << QJsonValue()              << QPairStringInt() << false;
    QTest::newRow("bool")          << QJsonValue(true)          << QPairStringInt() << false;
    QTest::newRow("double")        << QJsonValue(123.4)         << QPairStringInt() << false;
    QTest::newRow("string")        << QJsonValue("abc123")      << QPairStringInt() << false;
    QTest::newRow("array")         << QJsonValue(QJsonArray())  << QPairStringInt() << false;
    QTest::newRow("object: empty") << QJsonValue(QJsonObject()) << QPairStringInt() << false;

    {
        const QJsonObject value
        {
            { "second", 456 }
        };
        QTest::newRow("object: missing param 1") << QJsonValue(value) << QPairStringInt() << false;
    }

    {
        const QJsonObject value
        {
            { "first",  "abc" }
        };
        QTest::newRow("object: missing param 2") << QJsonValue(value) << QPairStringInt() << false;
    }

    {
        const QJsonObject value
        {
            { "first",  0 },
            { "second", 456 }
        };
        QTest::newRow("object: invalid param 1") << QJsonValue(value) << QPairStringInt() << false;
    }

    {
        const QJsonObject value
        {
            { "first",  "abc" },
            { "second", "abc" }
        };
        QTest::newRow("object: invalid param 2") << QJsonValue(value) << QPairStringInt() << false;
    }
}

// Test: deserialize<std::pair>() method -----------------------------------------------------------

using StdPairStringInt = std::pair<QString, int>;

void TestDeserialization::testDeserializeStdPair()
{
    QFETCH(QJsonValue, input);
    QFETCH(StdPairStringInt, expectedOutput);
    QFETCH(bool, expectedResult);

    StdPairStringInt output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeStdPair_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<StdPairStringInt>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    {
        const QJsonObject value
        {
            { "first",  "asd" },
            { "second", 123 },
        };
        QTest::newRow("object: 1") << QJsonValue(value) << StdPairStringInt("asd", 123) << true;
    }

    {
        const QJsonObject value
        {
            { "first",  "abc" },
            { "second", 456 },
        };
        QTest::newRow("object: 2") << QJsonValue(value) << StdPairStringInt("abc", 456) << true;
    }

    // Negative tests
    QTest::newRow("null")          << QJsonValue()              << StdPairStringInt() << false;
    QTest::newRow("bool")          << QJsonValue(true)          << StdPairStringInt() << false;
    QTest::newRow("double")        << QJsonValue(123.4)         << StdPairStringInt() << false;
    QTest::newRow("string")        << QJsonValue("abc123")      << StdPairStringInt() << false;
    QTest::newRow("array")         << QJsonValue(QJsonArray())  << StdPairStringInt() << false;
    QTest::newRow("object: empty") << QJsonValue(QJsonObject()) << StdPairStringInt() << false;

    {
        const QJsonObject value
        {
            { "second", 456 }
        };
        QTest::newRow("object: missing param 1")
                << QJsonValue(value) << StdPairStringInt() << false;
    }

    {
        const QJsonObject value
        {
            { "first",  "abc" }
        };
        QTest::newRow("object: missing param 2")
                << QJsonValue(value) << StdPairStringInt() << false;
    }

    {
        const QJsonObject value
        {
            { "first",  0 },
            { "second", 456 }
        };
        QTest::newRow("object: invalid param 1")
                << QJsonValue(value) << StdPairStringInt() << false;
    }

    {
        const QJsonObject value
        {
            { "first",  "abc" },
            { "second", "abc" }
        };
        QTest::newRow("object: invalid param 2")
                << QJsonValue(value) << StdPairStringInt() << false;
    }
}

// Test: deserialize<QList>() method ---------------------------------------------------------------

void TestDeserialization::testDeserializeQList()
{
    QFETCH(QJsonValue, input);
    QFETCH(QList<int>, expectedOutput);
    QFETCH(bool, expectedResult);

    QList<int> output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeQList_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QList<int>>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    QTest::newRow("array: empty") << QJsonValue(QJsonArray()) << QList<int>() << true;

    {
        const QList<int> expectedOutput { -1, 0, 1 };
        QTest::newRow("array: non-empty")
                << QJsonValue(QJsonArray { -1, 0, 1 } ) << expectedOutput << true;
    }

    // Negative tests
    QTest::newRow("null")   << QJsonValue()              << QList<int>() << false;
    QTest::newRow("bool")   << QJsonValue(true)          << QList<int>() << false;
    QTest::newRow("double") << QJsonValue(123.4)         << QList<int>() << false;
    QTest::newRow("string") << QJsonValue("abc123")      << QList<int>() << false;
    QTest::newRow("object") << QJsonValue(QJsonObject()) << QList<int>() << false;

    QTest::newRow("array: invalid") << QJsonValue(QJsonArray {"asd"}) << QList<int>() << false;
}

// Test: deserialize<std::list>() method -----------------------------------------------------------

void TestDeserialization::testDeserializeStdList()
{
    QFETCH(QJsonValue, input);
    QFETCH(std::list<int>, expectedOutput);
    QFETCH(bool, expectedResult);

    std::list<int> output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeStdList_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<std::list<int>>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    QTest::newRow("array: empty") << QJsonValue(QJsonArray()) << std::list<int>() << true;

    {
        const std::list<int> expectedOutput { -1, 0, 1 };
        QTest::newRow("array: non-empty")
                << QJsonValue(QJsonArray { -1, 0, 1 } ) << expectedOutput << true;
    }

    // Negative tests
    QTest::newRow("null")   << QJsonValue()              << std::list<int>() << false;
    QTest::newRow("bool")   << QJsonValue(true)          << std::list<int>() << false;
    QTest::newRow("double") << QJsonValue(123.4)         << std::list<int>() << false;
    QTest::newRow("string") << QJsonValue("abc123")      << std::list<int>() << false;
    QTest::newRow("object") << QJsonValue(QJsonObject()) << std::list<int>() << false;

    QTest::newRow("array: invalid") << QJsonValue(QJsonArray {"asd"}) << std::list<int>() << false;
}

// Test: deserialize<QVector>() method -------------------------------------------------------------

void TestDeserialization::testDeserializeQVector()
{
    QFETCH(QJsonValue, input);
    QFETCH(QVector<int>, expectedOutput);
    QFETCH(bool, expectedResult);

    QVector<int> output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeQVector_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QVector<int>>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    QTest::newRow("array: empty") << QJsonValue(QJsonArray()) << QVector<int>() << true;

    {
        const QVector<int> expectedOutput { -1, 0, 1 };
        QTest::newRow("array: non-empty")
                << QJsonValue(QJsonArray { -1, 0, 1 } ) << expectedOutput << true;
    }

    // Negative tests
    QTest::newRow("null")   << QJsonValue()              << QVector<int>() << false;
    QTest::newRow("bool")   << QJsonValue(true)          << QVector<int>() << false;
    QTest::newRow("double") << QJsonValue(123.4)         << QVector<int>() << false;
    QTest::newRow("string") << QJsonValue("abc123")      << QVector<int>() << false;
    QTest::newRow("object") << QJsonValue(QJsonObject()) << QVector<int>() << false;

    QTest::newRow("array: invalid") << QJsonValue(QJsonArray {"asd"}) << QVector<int>() << false;
}

// Test: deserialize<std::vector>() method ---------------------------------------------------------

void TestDeserialization::testDeserializeStdVector()
{
    QFETCH(QJsonValue, input);
    QFETCH(std::vector<int>, expectedOutput);
    QFETCH(bool, expectedResult);

    std::vector<int> output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeStdVector_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<std::vector<int>>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    QTest::newRow("array: empty") << QJsonValue(QJsonArray()) << std::vector<int>() << true;

    {
        const std::vector<int> expectedOutput { -1, 0, 1 };
        QTest::newRow("array: non-empty")
                << QJsonValue(QJsonArray { -1, 0, 1 } ) << expectedOutput << true;
    }

    // Negative tests
    QTest::newRow("null")   << QJsonValue()              << std::vector<int>() << false;
    QTest::newRow("bool")   << QJsonValue(true)          << std::vector<int>() << false;
    QTest::newRow("double") << QJsonValue(123.4)         << std::vector<int>() << false;
    QTest::newRow("string") << QJsonValue("abc123")      << std::vector<int>() << false;
    QTest::newRow("object") << QJsonValue(QJsonObject()) << std::vector<int>() << false;

    QTest::newRow("array: invalid")
            << QJsonValue(QJsonArray {"asd"}) << std::vector<int>() << false;
}

// Test: deserialize<QSet>() method ----------------------------------------------------------------

void TestDeserialization::testDeserializeQSet()
{
    QFETCH(QJsonValue, input);
    QFETCH(QSet<int>, expectedOutput);
    QFETCH(bool, expectedResult);

    QSet<int> output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeQSet_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QSet<int>>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    QTest::newRow("array: empty") << QJsonValue(QJsonArray()) << QSet<int>() << true;

    {
        const QSet<int> expectedOutput { -1, 0, 1 };
        QTest::newRow("array: non-empty")
                << QJsonValue(QJsonArray { -1, 0, 1 } ) << expectedOutput << true;
    }

    // Negative tests
    QTest::newRow("null")   << QJsonValue()              << QSet<int>() << false;
    QTest::newRow("bool")   << QJsonValue(true)          << QSet<int>() << false;
    QTest::newRow("double") << QJsonValue(123.4)         << QSet<int>() << false;
    QTest::newRow("string") << QJsonValue("abc123")      << QSet<int>() << false;
    QTest::newRow("object") << QJsonValue(QJsonObject()) << QSet<int>() << false;

    QTest::newRow("array: invalid 1") << QJsonValue(QJsonArray {"asd"})  << QSet<int>() << false;
    QTest::newRow("array: invalid 2") << QJsonValue(QJsonArray { 1, 1 }) << QSet<int>() << false;
}

// Test: deserialize<QMap>() method ----------------------------------------------------------------

using QMapIntString = QMap<int, QString>;

void TestDeserialization::testDeserializeQMap()
{
    QFETCH(QJsonValue, input);
    QFETCH(QMapIntString, expectedOutput);
    QFETCH(bool, expectedResult);

    QMapIntString output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeQMap_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QMapIntString>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    QTest::newRow("object: empty") << QJsonValue(QJsonObject()) << QMapIntString() << true;

    {
        const QMapIntString expectedOutput
        {
            { -1, "a" },
            {  0, "b" },
            {  1, "c" }
        };
        const QJsonObject input
        {
            { "-1", "a" },
            {  "0", "b" },
            {  "1", "c" }
        };
        QTest::newRow("object: non-empty") << QJsonValue(input) << expectedOutput << true;
    }

    // Negative tests
    QTest::newRow("null")   << QJsonValue()             << QMapIntString() << false;
    QTest::newRow("bool")   << QJsonValue(true)         << QMapIntString() << false;
    QTest::newRow("double") << QJsonValue(123.4)        << QMapIntString() << false;
    QTest::newRow("string") << QJsonValue("abc123")     << QMapIntString() << false;
    QTest::newRow("array")  << QJsonValue(QJsonArray()) << QMapIntString() << false;

    QTest::newRow("object: invalid key")
            << QJsonValue(QJsonObject {{"asd", "asd"}}) << QMapIntString() << false;

    QTest::newRow("object: invalid value")
            << QJsonValue(QJsonObject {{"0", 0}}) << QMapIntString() << false;
}

// Test: deserialize<std::map>() method ------------------------------------------------------------

using StdMapIntString = std::map<int, QString>;

void TestDeserialization::testDeserializeStdMap()
{
    QFETCH(QJsonValue, input);
    QFETCH(StdMapIntString, expectedOutput);
    QFETCH(bool, expectedResult);

    StdMapIntString output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeStdMap_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<StdMapIntString>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    QTest::newRow("object: empty") << QJsonValue(QJsonObject()) << StdMapIntString() << true;

    {
        const StdMapIntString expectedOutput
        {
            { -1, "a" },
            {  0, "b" },
            {  1, "c" }
        };
        const QJsonObject input
        {
            { "-1", "a" },
            {  "0", "b" },
            {  "1", "c" }
        };
        QTest::newRow("object: non-empty") << QJsonValue(input) << expectedOutput << true;
    }

    // Negative tests
    QTest::newRow("null")   << QJsonValue()             << StdMapIntString() << false;
    QTest::newRow("bool")   << QJsonValue(true)         << StdMapIntString() << false;
    QTest::newRow("double") << QJsonValue(123.4)        << StdMapIntString() << false;
    QTest::newRow("string") << QJsonValue("abc123")     << StdMapIntString() << false;
    QTest::newRow("array")  << QJsonValue(QJsonArray()) << StdMapIntString() << false;

    QTest::newRow("object: invalid key")
            << QJsonValue(QJsonObject {{"asd", "asd"}}) << StdMapIntString() << false;

    QTest::newRow("object: invalid value")
            << QJsonValue(QJsonObject {{"0", 0}}) << StdMapIntString() << false;
}

// Test: deserialize<QHash>() method ---------------------------------------------------------------

using QHashQDateString = QHash<QDate, QString>;

void TestDeserialization::testDeserializeQHash()
{
    QFETCH(QJsonValue, input);
    QFETCH(QHashQDateString, expectedOutput);
    QFETCH(bool, expectedResult);

    QHashQDateString output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeQHash_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QHashQDateString>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    QTest::newRow("object: empty") << QJsonValue(QJsonObject()) << QHashQDateString() << true;

    {
        const QHashQDateString expectedOutput
        {
            { QDate(2020, 1, 1), "a" },
            { QDate(2020, 1, 2), "b" },
            { QDate(2020, 1, 3), "c" }
        };
        const QJsonObject input
        {
            { "2020-01-01", "a" },
            { "2020-01-02", "b" },
            { "2020-01-03", "c" }
        };
        QTest::newRow("non-empty") << QJsonValue(input) << expectedOutput << true;
    }

    // Negative tests
    QTest::newRow("null")   << QJsonValue()             << QHashQDateString() << false;
    QTest::newRow("bool")   << QJsonValue(true)         << QHashQDateString() << false;
    QTest::newRow("double") << QJsonValue(123.4)        << QHashQDateString() << false;
    QTest::newRow("string") << QJsonValue("abc123")     << QHashQDateString() << false;
    QTest::newRow("array")  << QJsonValue(QJsonArray()) << QHashQDateString() << false;

    QTest::newRow("object: invalid key")
            << QJsonValue(QJsonObject {{"asd", "asd"}}) << QHashQDateString() << false;

    QTest::newRow("object: invalid value")
            << QJsonValue(QJsonObject {{"2020-01-03", 0}}) << QHashQDateString() << false;
}

// Test: deserialize<std::unordered_map>() method --------------------------------------------------

using StdUnorderedMapIntString = std::unordered_map<int, QString>;
Q_DECLARE_METATYPE(StdUnorderedMapIntString)

void TestDeserialization::testDeserializeStdUnorderedMap()
{
    QFETCH(QJsonValue, input);
    QFETCH(StdUnorderedMapIntString, expectedOutput);
    QFETCH(bool, expectedResult);

    StdUnorderedMapIntString output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeStdUnorderedMap_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<StdUnorderedMapIntString>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    QTest::newRow("object: empty")
            << QJsonValue(QJsonObject()) << StdUnorderedMapIntString() << true;

    {
        const StdUnorderedMapIntString expectedOutput
        {
            { -1, "a" },
            {  0, "b" },
            {  1, "c" }
        };
        const QJsonObject input
        {
            { "-1", "a" },
            {  "0", "b" },
            {  "1", "c" }
        };
        QTest::newRow("object: non-empty") << QJsonValue(input) << expectedOutput << true;
    }

    // Negative tests
    QTest::newRow("null")   << QJsonValue()             << StdUnorderedMapIntString() << false;
    QTest::newRow("bool")   << QJsonValue(true)         << StdUnorderedMapIntString() << false;
    QTest::newRow("double") << QJsonValue(123.4)        << StdUnorderedMapIntString() << false;
    QTest::newRow("string") << QJsonValue("abc123")     << StdUnorderedMapIntString() << false;
    QTest::newRow("array")  << QJsonValue(QJsonArray()) << StdUnorderedMapIntString() << false;

    QTest::newRow("object: invalid key")
            << QJsonValue(QJsonObject {{"asd", "asd"}}) << StdUnorderedMapIntString() << false;

    QTest::newRow("object: invalid value")
            << QJsonValue(QJsonObject {{"0", 0}}) << StdUnorderedMapIntString() << false;
}

// Test: deserialize<QMultiMap>() method -----------------------------------------------------------

using QMultiMapIntString = QMultiMap<int, QString>;
Q_DECLARE_METATYPE(QMultiMapIntString)

void TestDeserialization::testDeserializeQMultiMap()
{
    QFETCH(QJsonValue, input);
    QFETCH(QMultiMapIntString, expectedOutput);
    QFETCH(bool, expectedResult);

    QMultiMapIntString output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeQMultiMap_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QMultiMapIntString>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    QTest::newRow("object: empty") << QJsonValue(QJsonObject()) << QMultiMapIntString() << true;

    {
        const QMultiMapIntString expectedResult
        {
            { -1, "a1" },
            { -1, "a2" },
            { -1, "a3" },
            {  0, "b1" },
            {  0, "b2" },
            {  0, "b3" },
            {  1, "c1" },
            {  1, "c2" },
            {  1, "c3" }
        };
        const QJsonObject input
        {
            { "-1", QJsonArray { "a1", "a2", "a3" } },
            {  "0", QJsonArray { "b1", "b2", "b3" } },
            {  "1", QJsonArray { "c1", "c2", "c3" } }
        };
        QTest::newRow("non-empty") << QJsonValue(input) << expectedResult << true;
    }

    // Negative tests
    QTest::newRow("null")   << QJsonValue()             << QMultiMapIntString() << false;
    QTest::newRow("bool")   << QJsonValue(true)         << QMultiMapIntString() << false;
    QTest::newRow("double") << QJsonValue(123.4)        << QMultiMapIntString() << false;
    QTest::newRow("string") << QJsonValue("abc123")     << QMultiMapIntString() << false;
    QTest::newRow("array")  << QJsonValue(QJsonArray()) << QMultiMapIntString() << false;

    QTest::newRow("object: invalid key")
            << QJsonValue(QJsonObject {{"asd", "asd"}}) << QMultiMapIntString() << false;

    QTest::newRow("object: invalid value")
            << QJsonValue(QJsonObject {{"0", 0}}) << QMultiMapIntString() << false;
}

// Test: deserialize<QMultiHash>() method ----------------------------------------------------------

using QMultiHashIntString = QMultiHash<int, QString>;
Q_DECLARE_METATYPE(QMultiHashIntString)

void TestDeserialization::testDeserializeQMultiHash()
{
    QFETCH(QJsonValue, input);
    QFETCH(QMultiHashIntString, expectedOutput);
    QFETCH(bool, expectedResult);

    QMultiHashIntString output;
    const bool result = CedarFramework::deserialize(input, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeQMultiHash_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QMultiHashIntString>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    // Positive tests
    QTest::newRow("object: empty") << QJsonValue(QJsonObject()) << QMultiHashIntString() << true;

    {
        const QMultiHashIntString expectedResult
        {
            { -1, "a1" },
            { -1, "a2" },
            { -1, "a3" },
            {  0, "b1" },
            {  0, "b2" },
            {  0, "b3" },
            {  1, "c1" },
            {  1, "c2" },
            {  1, "c3" }
        };
        const QJsonObject input
        {
            { "-1", QJsonArray { "a1", "a2", "a3" } },
            {  "0", QJsonArray { "b1", "b2", "b3" } },
            {  "1", QJsonArray { "c1", "c2", "c3" } }
        };
        QTest::newRow("non-empty") << QJsonValue(input) << expectedResult << true;
    }

    // Negative tests
    QTest::newRow("null")   << QJsonValue()             << QMultiHashIntString() << false;
    QTest::newRow("bool")   << QJsonValue(true)         << QMultiHashIntString() << false;
    QTest::newRow("double") << QJsonValue(123.4)        << QMultiHashIntString() << false;
    QTest::newRow("string") << QJsonValue("abc123")     << QMultiHashIntString() << false;
    QTest::newRow("array")  << QJsonValue(QJsonArray()) << QMultiHashIntString() << false;

    QTest::newRow("object: invalid key")
            << QJsonValue(QJsonObject {{"asd", "asd"}}) << QMultiHashIntString() << false;

    QTest::newRow("object: invalid value")
            << QJsonValue(QJsonObject {{"0", 0}}) << QMultiHashIntString() << false;
}

// Test: deserializeNode(index) method -------------------------------------------------------------

void TestDeserialization::testDeserializeNodeByIndex()
{
    QFETCH(QJsonValue, input);
    QFETCH(int, index);
    QFETCH(QDate, expectedOutput);
    QFETCH(bool, expectedResult);

    QDate output;
    const bool result = CedarFramework::deserializeNode(input, index, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeNodeByIndex_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<int>("index");
    QTest::addColumn<QDate>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    const QJsonArray input
    {
        { 1.0 },
        { "2020-01-01" },
        { "2020-01-02" },
        { "asd" }
    };

    // Positive tests
    QTest::newRow("array: valid 1") << QJsonValue(input) << 1 << QDate(2020, 1, 1) << true;
    QTest::newRow("array: valid 2") << QJsonValue(input) << 2 << QDate(2020, 1, 2) << true;

    // Negative tests
    QTest::newRow("null")   << QJsonValue()         << 0 << QDate() << false;
    QTest::newRow("bool")   << QJsonValue(true)     << 0 << QDate() << false;
    QTest::newRow("double") << QJsonValue(123.4)    << 0 << QDate() << false;
    QTest::newRow("string") << QJsonValue("abc123") << 0 << QDate() << false;

    QTest::newRow("object") << QJsonValue(QJsonObject { {"a", 1} }) << 0 << QDate() << false;

    QTest::newRow("array: invalid -1") << QJsonValue(input) << -1 << QDate() << false;
    QTest::newRow("array: invalid  0") << QJsonValue(input) <<  0 << QDate() << false;
    QTest::newRow("array: invalid  3") << QJsonValue(input) <<  3 << QDate() << false;
    QTest::newRow("array: invalid  4") << QJsonValue(input) <<  4 << QDate() << false;
}

// Test: deserializeNode(name) method --------------------------------------------------------------

void TestDeserialization::testDeserializeNodeByName()
{
    QFETCH(QJsonValue, input);
    QFETCH(QString, name);
    QFETCH(QDate, expectedOutput);
    QFETCH(bool, expectedResult);

    QDate output;
    const bool result = CedarFramework::deserializeNode(input, name, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeNodeByName_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QString>("name");
    QTest::addColumn<QDate>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    const QJsonObject input
    {
        { "x", 1.0 },
        { "y", "2020-01-01" },
        { "w", "2020-01-02" },
        { "z", "asd" }
    };

    // Positive tests
    QTest::newRow("object: valid 'y'")
            << QJsonValue(input) << QString("y") << QDate(2020, 1, 1) << true;
    QTest::newRow("object: valid 'w'")
            << QJsonValue(input) << QString("w") << QDate(2020, 1, 2) << true;

    // Negative tests
    QTest::newRow("null")   << QJsonValue()         << QString("a") << QDate() << false;
    QTest::newRow("bool")   << QJsonValue(true)     << QString("a") << QDate() << false;
    QTest::newRow("double") << QJsonValue(123.4)    << QString("a") << QDate() << false;
    QTest::newRow("string") << QJsonValue("abc123") << QString("a") << QDate() << false;

    QTest::newRow("array") << QJsonValue(QJsonArray {"a", 1}) << QString("a") << QDate() << false;

    QTest::newRow("object: invalid 'a'") << QJsonValue(input) << QString("a") << QDate() << false;
    QTest::newRow("object: invalid 'x'") << QJsonValue(input) << QString("x") << QDate() << false;
    QTest::newRow("object: invalid 'z'") << QJsonValue(input) << QString("z") << QDate() << false;
    QTest::newRow("object: invalid ''")  << QJsonValue(input) << QString("")  << QDate() << false;
}

// Test: deserializeNode(path) method --------------------------------------------------------------

void TestDeserialization::testDeserializeNodeByPathVariantList()
{
    QFETCH(QJsonValue, input);
    QFETCH(QVariantList, path);
    QFETCH(QDate, expectedOutput);
    QFETCH(bool, expectedResult);

    QDate output;
    const bool result = CedarFramework::deserializeNode(input, path, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeNodeByPathVariantList_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QVariantList>("path");
    QTest::addColumn<QDate>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    const QJsonArray inputArray
    {
        1,
        QJsonObject
        {
            { "x", QJsonArray { 1, "2020-01-01", 3 } },
            { "y", QJsonObject { { "a", 1 }, { "b", "2020-01-01" } } },
            { "z", "z" }
        },
        true
    };

    const QJsonObject inputObject
    {
        { "a", true },
        {
            "b", QJsonArray
            {
                1,
                QJsonObject
                {
                    { "x", QJsonArray { 1, "2020-01-01", 3 } },
                    { "y", QJsonObject { { "a", 1 }, { "b", "2020-01-01" } } },
                    { "z", "z" }
                },
                true
            }
        },
        { "c", "2020-01-01" }
    };

    // Positive tests
    QTest::newRow("array: valid '1/x/1'")
            << QJsonValue(inputArray) << QVariantList {1, "x", 1}    << QDate(2020, 1, 1) << true;
    QTest::newRow("array: valid '1/y/b'")
            << QJsonValue(inputArray) << QVariantList { 1, "y", "b"} << QDate(2020, 1, 1) << true;

    QTest::newRow("object: valid 'b/1/x/1'")
            << QJsonValue(inputObject)
            << QVariantList {"b", 1, "x", 1}
            << QDate(2020, 1, 1)
            << true;
    QTest::newRow("object: valid 'b/1/y/b'")
            << QJsonValue(inputObject)
            << QVariantList {"b", 1, "y", "b"}
            << QDate(2020, 1, 1)
            << true;
    QTest::newRow("object: valid 'c'")
            << QJsonValue(inputObject)
            << QVariantList {"c"}
            << QDate(2020, 1, 1)
            << true;

    // Negative tests
    QTest::newRow("null")   << QJsonValue()         << QVariantList {0} << QDate() << false;
    QTest::newRow("bool")   << QJsonValue(true)     << QVariantList {0} << QDate() << false;
    QTest::newRow("double") << QJsonValue(123.4)    << QVariantList {0} << QDate() << false;
    QTest::newRow("string") << QJsonValue("abc123") << QVariantList {0} << QDate() << false;

    QTest::newRow("array: invalid -1")
            << QJsonValue(inputArray) << QVariantList {-1}          << QDate() << false;
    QTest::newRow("array: invalid 0")
            << QJsonValue(inputArray) << QVariantList {0}           << QDate() << false;
    QTest::newRow("array: invalid '1/x/0'")
            << QJsonValue(inputArray) << QVariantList {1, "x", 0}   << QDate() << false;
    QTest::newRow("array: invalid '1/y/a'")
            << QJsonValue(inputArray) << QVariantList {1, "y", "a"} << QDate() << false;
    QTest::newRow("array: invalid 3")
            << QJsonValue(inputArray) << QVariantList {3}           << QDate() << false;

    QTest::newRow("object: invalid ''")
            << QJsonValue(inputObject) << QVariantList {""}             << QDate() << false;
    QTest::newRow("object: invalid 'a'")
            << QJsonValue(inputObject) << QVariantList {"a"}            << QDate() << false;
    QTest::newRow("object: invalid 'd'")
            << QJsonValue(inputObject) << QVariantList {"d"}            << QDate() << false;
    QTest::newRow("object: invalid 'b/0'")
            << QJsonValue(inputObject) << QVariantList {"b", 0}         << QDate() << false;
    QTest::newRow("object: invalid 'b/1/x/0'")
            << QJsonValue(inputObject) << QVariantList {"b", 1, "x", 0} << QDate() << false;
}

// Test: deserializeNode(path) method --------------------------------------------------------------

void TestDeserialization::testDeserializeNodeByPathStringList()
{
    QFETCH(QJsonValue, input);
    QFETCH(QStringList, path);
    QFETCH(QDate, expectedOutput);
    QFETCH(bool, expectedResult);

    QDate output;
    const bool result = CedarFramework::deserializeNode(input, path, &output);
    QCOMPARE(result, expectedResult);

    if (result)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeNodeByPathStringList_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QStringList>("path");
    QTest::addColumn<QDate>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");

    const QJsonArray inputArray
    {
        1,
        QJsonObject
        {
            { "x", QJsonArray { 1, "2020-01-01", 3 } },
            { "y", QJsonObject { { "a", 1 }, { "b", "2020-01-01" } } },
            { "z", "z" }
        },
        true
    };

    const QJsonObject inputObject
    {
        { "a", true },
        {
            "b", QJsonArray
            {
                1,
                QJsonObject
                {
                    { "x", QJsonArray { 1, "2020-01-01", 3 } },
                    { "y", QJsonObject { { "a", 1 }, { "b", "2020-01-01" } } },
                    { "z", "z" }
                },
                true
            }
        },
        { "c", "2020-01-01" }
    };

    // Positive tests
    QTest::newRow("array: valid '1/x/1'")
            << QJsonValue(inputArray) << QStringList {"1", "x", "1"} << QDate(2020, 1, 1) << true;
    QTest::newRow("array: valid '1/y/b'")
            << QJsonValue(inputArray) << QStringList {"1", "y", "b"} << QDate(2020, 1, 1) << true;

    QTest::newRow("object: valid 'b/1/x/1'")
            << QJsonValue(inputObject)
            << QStringList {"b", "1", "x", "1"}
            << QDate(2020, 1, 1)
            << true;
    QTest::newRow("object: valid 'b/1/y/b'")
            << QJsonValue(inputObject)
            << QStringList {"b", "1", "y", "b"}
            << QDate(2020, 1, 1)
            << true;
    QTest::newRow("object: valid 'c'")
            << QJsonValue(inputObject)
            << QStringList {"c"}
            << QDate(2020, 1, 1)
            << true;

    // Negative tests
    QTest::newRow("null")   << QJsonValue()         << QStringList {"0"} << QDate() << false;
    QTest::newRow("bool")   << QJsonValue(true)     << QStringList {"0"} << QDate() << false;
    QTest::newRow("double") << QJsonValue(123.4)    << QStringList {"0"} << QDate() << false;
    QTest::newRow("string") << QJsonValue("abc123") << QStringList {"0"} << QDate() << false;

    QTest::newRow("array: invalid -1")
            << QJsonValue(inputArray) << QStringList {"-1"}          << QDate() << false;
    QTest::newRow("array: invalid 0")
            << QJsonValue(inputArray) << QStringList {"0"}           << QDate() << false;
    QTest::newRow("array: invalid '1/x/0'")
            << QJsonValue(inputArray) << QStringList {"1", "x", "0"} << QDate() << false;
    QTest::newRow("array: invalid '1/y/a'")
            << QJsonValue(inputArray) << QStringList {"1", "y", "a"} << QDate() << false;
    QTest::newRow("array: invalid 3")
            << QJsonValue(inputArray) << QStringList {"3"}           << QDate() << false;

    QTest::newRow("object: invalid ''")
            << QJsonValue(inputObject) << QStringList {""}                 << QDate() << false;
    QTest::newRow("object: invalid 'a'")
            << QJsonValue(inputObject) << QStringList {"a"}                << QDate() << false;
    QTest::newRow("object: invalid 'd'")
            << QJsonValue(inputObject) << QStringList {"d"}                << QDate() << false;
    QTest::newRow("object: invalid 'b/0'")
            << QJsonValue(inputObject) << QStringList {"b", "0"}           << QDate() << false;
    QTest::newRow("object: invalid 'b/1/x/0'")
            << QJsonValue(inputObject) << QStringList {"b", "1", "x", "0"} << QDate() << false;
}

// Test: deserializeOptionalNode(index) method -----------------------------------------------------

void TestDeserialization::testDeserializeOptionalNodeByIndex()
{
    QFETCH(QJsonValue, input);
    QFETCH(int, index);
    QFETCH(QDate, expectedOutput);
    QFETCH(bool, expectedResult);
    QFETCH(bool, expectedDeserialized);

    QDate output;
    bool deserialized;
    const bool result =
            CedarFramework::deserializeOptionalNode(input, index, &output, &deserialized);
    QCOMPARE(result, expectedResult);
    QCOMPARE(deserialized, expectedDeserialized);

    if (result && deserialized)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeOptionalNodeByIndex_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<int>("index");
    QTest::addColumn<QDate>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");
    QTest::addColumn<bool>("expectedDeserialized");

    const QJsonArray input
    {
        { 1.0 },
        { "2020-01-01" },
        { "2020-01-02" },
        { "asd" }
    };

    // Positive tests
    QTest::newRow("null")   << QJsonValue()         << 0 << QDate() << true << false;
    QTest::newRow("bool")   << QJsonValue(true)     << 0 << QDate() << true << false;
    QTest::newRow("double") << QJsonValue(123.4)    << 0 << QDate() << true << false;
    QTest::newRow("string") << QJsonValue("abc123") << 0 << QDate() << true << false;

    QTest::newRow("array: valid 1") << QJsonValue(input) << 1 << QDate(2020, 1, 1) << true << true;
    QTest::newRow("array: valid 2") << QJsonValue(input) << 2 << QDate(2020, 1, 2) << true << true;

    QTest::newRow("array: not deserialized -1")
            << QJsonValue(input) << -1 << QDate() << true << false;
    QTest::newRow("array: not deserialized  4")
            << QJsonValue(input) <<  4 << QDate() << true << false;

    QTest::newRow("object: not deserialized")
            << QJsonValue(QJsonObject { {"a", 1} }) << 0 << QDate() << true << false;

    // Negative tests
    QTest::newRow("array: invalid 0") << QJsonValue(input) << 0 << QDate() << false << false;
    QTest::newRow("array: invalid 3") << QJsonValue(input) << 3 << QDate() << false << false;
}

// Test: deserializeOptionalNode(name) method ------------------------------------------------------

void TestDeserialization::testDeserializeOptionalNodeByName()
{
    QFETCH(QJsonValue, input);
    QFETCH(QString, name);
    QFETCH(QDate, expectedOutput);
    QFETCH(bool, expectedResult);
    QFETCH(bool, expectedDeserialized);

    QDate output;
    bool deserialized;
    const bool result =
            CedarFramework::deserializeOptionalNode(input, name, &output, &deserialized);
    QCOMPARE(result, expectedResult);
    QCOMPARE(deserialized, expectedDeserialized);

    if (result && deserialized)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeOptionalNodeByName_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QString>("name");
    QTest::addColumn<QDate>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");
    QTest::addColumn<bool>("expectedDeserialized");

    const QJsonObject input
    {
        { "x", 1.0 },
        { "y", "2020-01-01" },
        { "w", "2020-01-02" },
        { "z", "asd" }
    };

    // Positive tests
    QTest::newRow("null")   << QJsonValue()         << QString("a") << QDate() << true << false;
    QTest::newRow("bool")   << QJsonValue(true)     << QString("a") << QDate() << true << false;
    QTest::newRow("double") << QJsonValue(123.4)    << QString("a") << QDate() << true << false;
    QTest::newRow("string") << QJsonValue("abc123") << QString("a") << QDate() << true << false;

    QTest::newRow("array: not deserialized")
            << QJsonValue(QJsonArray {"a", 1}) << QString("a") << QDate() << true << false;

    QTest::newRow("object: valid 'y'")
            << QJsonValue(input) << QString("y") << QDate(2020, 1, 1) << true << true;
    QTest::newRow("object: valid 'w'")
            << QJsonValue(input) << QString("w") << QDate(2020, 1, 2) << true << true;

    QTest::newRow("object: not deserialized 'a'")
            << QJsonValue(input) << QString("a") << QDate() << true << false;
    QTest::newRow("object: not deserialized ''")
            << QJsonValue(input) << QString("")  << QDate() << true << false;

    // Negative tests
    QTest::newRow("object: invalid 'x'")
            << QJsonValue(input) << QString("x") << QDate() << false << false;
    QTest::newRow("object: invalid 'z'")
            << QJsonValue(input) << QString("z") << QDate() << false << false;
}


// Test: deserializeOptionalNode(path) method ------------------------------------------------------

void TestDeserialization::testDeserializeOptionalNodeByPathVariantList()
{
    QFETCH(QJsonValue, input);
    QFETCH(QVariantList, path);
    QFETCH(QDate, expectedOutput);
    QFETCH(bool, expectedResult);
    QFETCH(bool, expectedDeserialized);

    QDate output;
    bool deserialized;
    const bool result =
            CedarFramework::deserializeOptionalNode(input, path, &output, &deserialized);
    QCOMPARE(result, expectedResult);
    QCOMPARE(deserialized, expectedDeserialized);

    if (result && deserialized)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeOptionalNodeByPathVariantList_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QVariantList>("path");
    QTest::addColumn<QDate>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");
    QTest::addColumn<bool>("expectedDeserialized");

    const QJsonArray inputArray
    {
        1,
        QJsonObject
        {
            { "x", QJsonArray { 1, "2020-01-01", 3 } },
            { "y", QJsonObject { { "a", 1 }, { "b", "2020-01-01" } } },
            { "z", "z" }
        },
        true
    };

    const QJsonObject inputObject
    {
        { "a", true },
        {
            "b", QJsonArray
            {
                1,
                QJsonObject
                {
                    { "x", QJsonArray { 1, "2020-01-01", 3 } },
                    { "y", QJsonObject { { "a", 1 }, { "b", "2020-01-01" } } },
                    { "z", "z" }
                },
                true
            }
        },
        { "c", "2020-01-01" }
    };

    // Positive tests
    QTest::newRow("null")   << QJsonValue()         << QVariantList {0} << QDate() << true << false;
    QTest::newRow("bool")   << QJsonValue(true)     << QVariantList {0} << QDate() << true << false;
    QTest::newRow("double") << QJsonValue(123.4)    << QVariantList {0} << QDate() << true << false;
    QTest::newRow("string") << QJsonValue("abc123") << QVariantList {0} << QDate() << true << false;

    QTest::newRow("array: valid '1/x/1'")
            << QJsonValue(inputArray)
            << QVariantList {1, "x", 1}
            << QDate(2020, 1, 1)
            << true
            << true;
    QTest::newRow("array: valid '1/y/b'")
            << QJsonValue(inputArray)
            << QVariantList { 1, "y", "b"}
            << QDate(2020, 1, 1)
            << true
            << true;

    QTest::newRow("object: valid 'b/1/x/1'")
            << QJsonValue(inputObject)
            << QVariantList {"b", 1, "x", 1}
            << QDate(2020, 1, 1)
            << true
            << true;
    QTest::newRow("object: valid 'b/1/y/b'")
            << QJsonValue(inputObject)
            << QVariantList {"b", 1, "y", "b"}
            << QDate(2020, 1, 1)
            << true
            << true;
    QTest::newRow("object: valid 'c'")
            << QJsonValue(inputObject)
            << QVariantList {"c"}
            << QDate(2020, 1, 1)
            << true
            << true;

    QTest::newRow("array: not deserialized -1")
            << QJsonValue(inputArray) << QVariantList {-1} << QDate() << true << false;
    QTest::newRow("array: not deserialized 3")
            << QJsonValue(inputArray) << QVariantList { 3} << QDate() << true << false;

    QTest::newRow("object: not deserialized ''")
            << QJsonValue(inputObject) << QVariantList {""}  << QDate() << true << false;
    QTest::newRow("object: not deserialized 'd'")
            << QJsonValue(inputObject) << QVariantList {"d"} << QDate() << true << false;

    // Negative tests
    QTest::newRow("array: invalid 0")
            << QJsonValue(inputArray) << QVariantList {0}           << QDate() << false << false;
    QTest::newRow("array: invalid '1/x/0'")
            << QJsonValue(inputArray) << QVariantList {1, "x", 0}   << QDate() << false << false;
    QTest::newRow("array: invalid '1/y/a'")
            << QJsonValue(inputArray) << QVariantList {1, "y", "a"} << QDate() << false << false;

    QTest::newRow("object: invalid 'a'")
            << QJsonValue(inputObject)
            << QVariantList {"a"}
            << QDate()
            << false
            << false;
    QTest::newRow("object: invalid 'b/0'")
            << QJsonValue(inputObject)
            << QVariantList {"b", 0}
            << QDate()
            << false
            << false;
    QTest::newRow("object: invalid 'b/1/x/0'")
            << QJsonValue(inputObject)
            << QVariantList {"b", 1, "x", 0}
            << QDate()
            << false
            << false;
}

// Test: deserializeOptionalNode(path) method ------------------------------------------------------

void TestDeserialization::testDeserializeOptionalNodeByPathStringList()
{
    QFETCH(QJsonValue, input);
    QFETCH(QStringList, path);
    QFETCH(QDate, expectedOutput);
    QFETCH(bool, expectedResult);
    QFETCH(bool, expectedDeserialized);

    QDate output;
    bool deserialized;
    const bool result =
            CedarFramework::deserializeOptionalNode(input, path, &output, &deserialized);
    QCOMPARE(result, expectedResult);
    QCOMPARE(deserialized, expectedDeserialized);

    if (result && deserialized)
    {
        QCOMPARE(output, expectedOutput);
    }
}

void TestDeserialization::testDeserializeOptionalNodeByPathStringList_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QStringList>("path");
    QTest::addColumn<QDate>("expectedOutput");
    QTest::addColumn<bool>("expectedResult");
    QTest::addColumn<bool>("expectedDeserialized");

    const QJsonArray inputArray
    {
        1,
        QJsonObject
        {
            { "x", QJsonArray { 1, "2020-01-01", 3 } },
            { "y", QJsonObject { { "a", 1 }, { "b", "2020-01-01" } } },
            { "z", "z" }
        },
        true
    };

    const QJsonObject inputObject
    {
        { "a", true },
        {
            "b", QJsonArray
            {
                1,
                QJsonObject
                {
                    { "x", QJsonArray { 1, "2020-01-01", 3 } },
                    { "y", QJsonObject { { "a", 1 }, { "b", "2020-01-01" } } },
                    { "z", "z" }
                },
                true
            }
        },
        { "c", "2020-01-01" }
    };

    // Positive tests
    QTest::newRow("null")
            << QJsonValue()         << QStringList {"0"} << QDate() << true << false;
    QTest::newRow("bool")
            << QJsonValue(true)     << QStringList {"0"} << QDate() << true << false;
    QTest::newRow("double")
            << QJsonValue(123.4)    << QStringList {"0"} << QDate() << true << false;
    QTest::newRow("string")
            << QJsonValue("abc123") << QStringList {"0"} << QDate() << true << false;

    QTest::newRow("array: valid '1/x/1'")
            << QJsonValue(inputArray)
            << QStringList {"1", "x", "1"}
            << QDate(2020, 1, 1)
            << true
            << true;
    QTest::newRow("array: valid '1/y/b'")
            << QJsonValue(inputArray)
            << QStringList {"1", "y", "b"}
            << QDate(2020, 1, 1)
            << true
            << true;

    QTest::newRow("object: valid 'b/1/x/1'")
            << QJsonValue(inputObject)
            << QStringList {"b", "1", "x", "1"}
            << QDate(2020, 1, 1)
            << true
            << true;
    QTest::newRow("object: valid 'b/1/y/b'")
            << QJsonValue(inputObject)
            << QStringList {"b", "1", "y", "b"}
            << QDate(2020, 1, 1)
            << true
            << true;
    QTest::newRow("object: valid 'c'")
            << QJsonValue(inputObject)
            << QStringList {"c"}
            << QDate(2020, 1, 1)
            << true
            << true;

    QTest::newRow("array: not deserialized -1")
            << QJsonValue(inputArray) << QStringList {"-1"} << QDate() << true << false;
    QTest::newRow("array: not deserialized 3")
            << QJsonValue(inputArray) << QStringList { "3"} << QDate() << true << false;

    QTest::newRow("object: not deserialized ''")
            << QJsonValue(inputObject) << QStringList {""}  << QDate() << true << false;
    QTest::newRow("object: not deserialized 'd'")
            << QJsonValue(inputObject) << QStringList {"d"} << QDate() << true << false;

    // Negative tests
    QTest::newRow("array: invalid 0")
            << QJsonValue(inputArray) << QStringList {"0"}           << QDate() << false << false;
    QTest::newRow("array: invalid '1/x/0'")
            << QJsonValue(inputArray) << QStringList {"1", "x", "0"} << QDate() << false << false;
    QTest::newRow("array: invalid '1/y/a'")
            << QJsonValue(inputArray) << QStringList {"1", "y", "a"} << QDate() << false << false;

    QTest::newRow("object: invalid 'a'")
            << QJsonValue(inputObject)
            << QStringList {"a"}
            << QDate()
            << false
            << false;
    QTest::newRow("object: invalid 'b/0'")
            << QJsonValue(inputObject)
            << QStringList {"b", "0"}
            << QDate()
            << false
            << false;
    QTest::newRow("object: invalid 'b/1/x/0'")
            << QJsonValue(inputObject)
            << QStringList {"b", "1", "x", "0"}
            << QDate()
            << false
            << false;
}

// Main function -----------------------------------------------------------------------------------

QTEST_MAIN(TestDeserialization)
#include "testDeserialization.moc"
