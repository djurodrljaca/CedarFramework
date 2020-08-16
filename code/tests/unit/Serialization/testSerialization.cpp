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
 * Contains unit tests for Serialization class
 */

// Cedar Framework includes
#include <CedarFramework/Serialization.hpp>

// Qt includes
#include <QtCore/QCborArray>
#include <QtCore/QCborMap>
#include <QtCore/QCborValue>
#include <QtCore/QDebug>
#include <QtCore/QJsonDocument>
#include <QtCore/QLine>
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
        QTest::newRow("min")
                << std::numeric_limits<T>::lowest()
                << QJsonValue(QString::number(std::numeric_limits<T>::lowest()));
        QTest::newRow("-1") << static_cast<T>(-1) << QJsonValue(-1LL);
        QTest::newRow(" 0") << static_cast<T>( 0) << QJsonValue( 0LL);
        QTest::newRow("+1") << static_cast<T>( 1) << QJsonValue( 1LL);
        QTest::newRow("max")
                << std::numeric_limits<T>::max()
                << QJsonValue(QString::number(std::numeric_limits<T>::max()));
    }
    else
    {
        QTest::newRow("0") << static_cast<T>(0) << QJsonValue(0LL);
        QTest::newRow("1") << static_cast<T>(1) << QJsonValue(1LL);
        QTest::newRow("max")
                << std::numeric_limits<T>::max()
                << QJsonValue(QString::number(std::numeric_limits<T>::max()));
    }
}

// -------------------------------------------------------------------------------------------------

template<typename T, Is64BitInteger<T> = true>
void createIntegerTestData()
{
    // Check if input value is signed
    if (std::is_signed<T>::value)
    {
        QTest::newRow("min")
                << std::numeric_limits<T>::lowest()
                << QJsonValue(QString::number(std::numeric_limits<T>::lowest()));
        QTest::newRow("-9007199254740993")
                << static_cast<T>(-9007199254740993LL)
                << QJsonValue(QString("-9007199254740993"));
        QTest::newRow("-9007199254740992")
                << static_cast<T>(-9007199254740992LL)
                << QJsonValue(-9007199254740992LL);
        QTest::newRow("-1") << static_cast<T>(-1LL) << QJsonValue(-1LL);
        QTest::newRow(" 0") << static_cast<T>( 0LL) << QJsonValue( 0LL);
        QTest::newRow("+1") << static_cast<T>( 1LL) << QJsonValue( 1LL);
        QTest::newRow("+9007199254740992")
                << static_cast<T>(9007199254740992LL)
                << QJsonValue(9007199254740992LL);
        QTest::newRow("+9007199254740993")
                << static_cast<T>(9007199254740993LL)
                << QJsonValue(QString("9007199254740993"));
        QTest::newRow("max")
                << std::numeric_limits<T>::max()
                << QJsonValue(QString::number(std::numeric_limits<T>::max()));
    }
    else
    {
        QTest::newRow("0") << static_cast<T>(0) << QJsonValue(0LL);
        QTest::newRow("1") << static_cast<T>(1) << QJsonValue(1LL);
        QTest::newRow("9007199254740992")
                << static_cast<T>(9007199254740992ULL)
                << QJsonValue(9007199254740992LL);
        QTest::newRow("9007199254740993")
                << static_cast<T>(9007199254740993ULL)
                << QJsonValue(QString("9007199254740993"));
        QTest::newRow("max")
                << std::numeric_limits<T>::max()
                << QJsonValue(QString::number(std::numeric_limits<T>::max()));
    }
}

// -------------------------------------------------------------------------------------------------

class TestSerialization : public QObject
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
    void testSerializeBool();
    void testSerializeBool_data();

    void testSerializeSignedChar();
    void testSerializeSignedChar_data();

    void testSerializeUnsignedChar();
    void testSerializeUnsignedChar_data();

    void testSerializeShort();
    void testSerializeShort_data();

    void testSerializeUnsignedShort();
    void testSerializeUnsignedShort_data();

    void testSerializeInt();
    void testSerializeInt_data();

    void testSerializeUnsignedInt();
    void testSerializeUnsignedInt_data();

    void testSerializeLong();
    void testSerializeLong_data();

    void testSerializeUnsignedLong();
    void testSerializeUnsignedLong_data();

    void testSerializeLongLong();
    void testSerializeLongLong_data();

    void testSerializeUnsignedLongLong();
    void testSerializeUnsignedLongLong_data();

    void testSerializeFloat();
    void testSerializeFloat_data();

    void testSerializeDouble();
    void testSerializeDouble_data();

    void testSerializeQChar();
    void testSerializeQChar_data();

    void testSerializeQString();
    void testSerializeQString_data();

    void testSerializeQByteArray();
    void testSerializeQByteArray_data();

    void testSerializeQBitArray();
    void testSerializeQBitArray_data();

    void testSerializeStdString();
    void testSerializeStdString_data();

    void testSerializeStdWString();
    void testSerializeStdWString_data();

    void testSerializeStdU16String();
    void testSerializeStdU16String_data();

    void testSerializeStdU32String();
    void testSerializeStdU32String_data();

    void testSerializeQDate();
    void testSerializeQDate_data();

    void testSerializeQTime();
    void testSerializeQTime_data();

    void testSerializeQDateTime();
    void testSerializeQDateTime_data();

    void testSerializeQVariant();
    void testSerializeQVariant_data();

    void testSerializeQUrl();
    void testSerializeQUrl_data();

    void testSerializeQUuid();
    void testSerializeQUuid_data();

    void testSerializeQLocale();
    void testSerializeQLocale_data();

    void testSerializeQRegExp();
    void testSerializeQRegExp_data();

    void testSerializeQRegularExpression();
    void testSerializeQRegularExpression_data();

    void testSerializeQSize();
    void testSerializeQSize_data();

    void testSerializeQSizeF();
    void testSerializeQSizeF_data();

    void testSerializeQPoint();
    void testSerializeQPoint_data();

    void testSerializeQPointF();
    void testSerializeQPointF_data();

    void testSerializeQLine();
    void testSerializeQLine_data();

    void testSerializeQLineF();
    void testSerializeQLineF_data();

    void testSerializeQRect();
    void testSerializeQRect_data();

    void testSerializeQRectF();
    void testSerializeQRectF_data();

    void testSerializeQStringList();
    void testSerializeQStringList_data();

    void testSerializeQJsonValue();
    void testSerializeQJsonValue_data();

    void testSerializeQJsonArray();
    void testSerializeQJsonArray_data();

    void testSerializeQJsonObject();
    void testSerializeQJsonObject_data();

    void testSerializeQJsonDocument();
    void testSerializeQJsonDocument_data();

    void testSerializeQCborValue();
    void testSerializeQCborValue_data();

    void testSerializeQCborArray();
    void testSerializeQCborArray_data();

    void testSerializeQCborMap();
    void testSerializeQCborMap_data();

    void testSerializeQCborSimpleType();
    void testSerializeQCborSimpleType_data();

    void testSerializeQPair();
    void testSerializeQPair_data();

    void testSerializeStdPair();
    void testSerializeStdPair_data();

    void testSerializeQList();
    void testSerializeQList_data();

    void testSerializeStdList();
    void testSerializeStdList_data();

    void testSerializeQVector();
    void testSerializeQVector_data();

    void testSerializeStdVector();
    void testSerializeStdVector_data();

    void testSerializeQSet();
    void testSerializeQSet_data();

    void testSerializeQMap();
    void testSerializeQMap_data();

    void testSerializeStdMap();
    void testSerializeStdMap_data();

    void testSerializeQHash();
    void testSerializeQHash_data();

    void testSerializeStdUnorderedMap();
    void testSerializeStdUnorderedMap_data();

    void testSerializeQMultiMap();
    void testSerializeQMultiMap_data();

    void testSerializeQMultiHash();
    void testSerializeQMultiHash_data();
};

// Test Case init/cleanup methods ------------------------------------------------------------------

void TestSerialization::initTestCase()
{
}

void TestSerialization::cleanupTestCase()
{
}

// Test init/cleanup methods -----------------------------------------------------------------------

void TestSerialization::init()
{
}

void TestSerialization::cleanup()
{
}

// Test: serialize<bool>() method ------------------------------------------------------------------

void TestSerialization::testSerializeBool()
{
    QFETCH(bool, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeBool_data()
{
    QTest::addColumn<bool>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    QTest::newRow("true") << true << QJsonValue(true);
    QTest::newRow("false") << false << QJsonValue(false);
}

// Test: serialize<signed char>() method -----------------------------------------------------------

void TestSerialization::testSerializeSignedChar()
{
    QFETCH(signed char, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeSignedChar_data()
{
    QTest::addColumn<signed char>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    QTest::newRow("min")
            << std::numeric_limits<signed char>::lowest()
            << QJsonValue(static_cast<int>(std::numeric_limits<signed char>::lowest()));
    QTest::newRow("-1") << static_cast<signed char>(-1) << QJsonValue(-1);
    QTest::newRow(" 0") << static_cast<signed char>( 0) << QJsonValue( 0);
    QTest::newRow("+1") << static_cast<signed char>( 1) << QJsonValue( 1);
    QTest::newRow("max")
            << std::numeric_limits<signed char>::max()
            << QJsonValue(static_cast<int>(std::numeric_limits<signed char>::max()));
}

// Test: serialize<unsigned char>() method ---------------------------------------------------------

void TestSerialization::testSerializeUnsignedChar()
{
    QFETCH(unsigned char, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeUnsignedChar_data()
{
    QTest::addColumn<unsigned char>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    QTest::newRow("0") << static_cast<unsigned char>(0) << QJsonValue(0);
    QTest::newRow("1") << static_cast<unsigned char>(1) << QJsonValue(1);
    QTest::newRow("max")
            << std::numeric_limits<unsigned char>::max()
            << QJsonValue(static_cast<int>(std::numeric_limits<unsigned char>::max()));
}

// Test: serialize<short>() method -----------------------------------------------------------------

void TestSerialization::testSerializeShort()
{
    QFETCH(short, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeShort_data()
{
    QTest::addColumn<short>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    QTest::newRow("min")
            << std::numeric_limits<short>::lowest()
            << QJsonValue(static_cast<int>(std::numeric_limits<short>::lowest()));
    QTest::newRow("-1") << static_cast<short>(-1) << QJsonValue(-1);
    QTest::newRow(" 0") << static_cast<short>( 0) << QJsonValue( 0);
    QTest::newRow("+1") << static_cast<short>( 1) << QJsonValue( 1);
    QTest::newRow("max")
            << std::numeric_limits<short>::max()
            << QJsonValue(static_cast<int>(std::numeric_limits<short>::max()));
}

// Test: serialize<unsigned short>() method --------------------------------------------------------

void TestSerialization::testSerializeUnsignedShort()
{
    QFETCH(unsigned short, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeUnsignedShort_data()
{
    QTest::addColumn<unsigned short>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    QTest::newRow("0") << static_cast<unsigned short>(0) << QJsonValue(0);
    QTest::newRow("1") << static_cast<unsigned short>(1) << QJsonValue(1);
    QTest::newRow("max")
            << std::numeric_limits<unsigned short>::max()
            << QJsonValue(static_cast<int>(std::numeric_limits<unsigned short>::max()));
}

// Test: serialize<int>() method -------------------------------------------------------------------

void TestSerialization::testSerializeInt()
{
    QFETCH(int, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeInt_data()
{
    QTest::addColumn<int>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    QTest::newRow("min")
            << std::numeric_limits<int>::lowest()
            << QJsonValue(static_cast<int>(std::numeric_limits<int>::lowest()));
    QTest::newRow("-1") << static_cast<int>(-1) << QJsonValue(-1);
    QTest::newRow(" 0") << static_cast<int>( 0) << QJsonValue( 0);
    QTest::newRow("+1") << static_cast<int>( 1) << QJsonValue( 1);
    QTest::newRow("max")
            << std::numeric_limits<int>::max()
            << QJsonValue(static_cast<int>(std::numeric_limits<int>::max()));
}

// Test: serialize<unsigned int>() method ----------------------------------------------------------

void TestSerialization::testSerializeUnsignedInt()
{
    QFETCH(unsigned int, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeUnsignedInt_data()
{
    QTest::addColumn<unsigned int>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    QTest::newRow("0") << static_cast<unsigned int>(0) << QJsonValue(0);
    QTest::newRow("1") << static_cast<unsigned int>(1) << QJsonValue(1);
    QTest::newRow("max")
            << std::numeric_limits<unsigned int>::max()
            << QJsonValue(static_cast<qint64>(std::numeric_limits<unsigned int>::max()));
}

// Test: serialize<long>() method ------------------------------------------------------------------

void TestSerialization::testSerializeLong()
{
    QFETCH(long, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeLong_data()
{
    QTest::addColumn<long>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    createIntegerTestData<long>();
}

// Test: serialize<unsigned long>() method ---------------------------------------------------------

void TestSerialization::testSerializeUnsignedLong()
{
    QFETCH(unsigned long, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeUnsignedLong_data()
{
    QTest::addColumn<unsigned long>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    createIntegerTestData<unsigned long>();
}

// Test: serialize<long long>() method -------------------------------------------------------------

void TestSerialization::testSerializeLongLong()
{
    QFETCH(long long, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeLongLong_data()
{
    QTest::addColumn<long long>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    createIntegerTestData<long long>();
}

// Test: serialize<unsigned long long>() method ----------------------------------------------------

void TestSerialization::testSerializeUnsignedLongLong()
{
    QFETCH(unsigned long long, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeUnsignedLongLong_data()
{
    QTest::addColumn<unsigned long long>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    createIntegerTestData<unsigned long long>();
}

// Test: serialize<float>() method -----------------------------------------------------------------

void TestSerialization::testSerializeFloat()
{
    QFETCH(float, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeFloat_data()
{
    QTest::addColumn<float>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    QTest::newRow("min") << std::numeric_limits<float>::lowest()
                         << QJsonValue(static_cast<double>(std::numeric_limits<float>::lowest()));
    QTest::newRow("-1.0") << -1.0f << QJsonValue(-1.0);
    QTest::newRow(" 0.0") <<  0.0f << QJsonValue( 0.0);
    QTest::newRow("+1.0") <<  1.0f << QJsonValue( 1.0);
    QTest::newRow("max") << std::numeric_limits<float>::max()
                         << QJsonValue(static_cast<double>(std::numeric_limits<float>::max()));
}

// Test: serialize<double>() method ----------------------------------------------------------------

void TestSerialization::testSerializeDouble()
{
    QFETCH(double, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeDouble_data()
{
    QTest::addColumn<double>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    QTest::newRow("min") << std::numeric_limits<double>::lowest()
                         << QJsonValue(std::numeric_limits<double>::lowest());
    QTest::newRow("-1.0") << -1.0 << QJsonValue(-1.0);
    QTest::newRow(" 0.0") <<  0.0 << QJsonValue( 0.0);
    QTest::newRow("+1.0") <<  1.0 << QJsonValue( 1.0);
    QTest::newRow("max") << std::numeric_limits<double>::max()
                         << QJsonValue(std::numeric_limits<double>::max());
}

// Test: serialize<QChar>() method -----------------------------------------------------------------

void TestSerialization::testSerializeQChar()
{
    QFETCH(QChar, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeQChar_data()
{
    QTest::addColumn<QChar>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    QTest::newRow("A") << QChar('A') << QJsonValue("A");
    QTest::newRow("z") << QChar('z') << QJsonValue("z");
    QTest::newRow("0") << QChar('0') << QJsonValue("0");
    QTest::newRow("9") << QChar('9') << QJsonValue("9");
    QTest::newRow("+") << QChar('+') << QJsonValue("+");
}

// Test: serialize<QString>() method ---------------------------------------------------------------

void TestSerialization::testSerializeQString()
{
    QFETCH(QString, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeQString_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    QTest::newRow("empty") << QString() << QJsonValue("");
    QTest::newRow("a z A Z 0 9 + - ?") << QString("a z A Z 0 9 + - ?")
                                       << QJsonValue("a z A Z 0 9 + - ?");
}

// Test: serialize<QByteArray>() method ------------------------------------------------------------

void TestSerialization::testSerializeQByteArray()
{
    QFETCH(QByteArray, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeQByteArray_data()
{
    QTest::addColumn<QByteArray>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    QTest::newRow("empty") << QByteArray() << QJsonValue("");
    QTest::newRow("a z A Z 0 9 + - ?") << QByteArray("a z A Z 0 9 + - ?")
                                       << QJsonValue("YSB6IEEgWiAwIDkgKyAtID8=");
}

// Test: serialize<QBitArray>() method -------------------------------------------------------------

void TestSerialization::testSerializeQBitArray()
{
    QFETCH(QBitArray, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);

    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeQBitArray_data()
{
    QTest::addColumn<QBitArray>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    {
        const QJsonObject expectedResult
        {
            { "bit_count",    0 },
            { "encoded_bits", "" }
        };
        QTest::newRow("empty") << QBitArray() << QJsonValue(expectedResult);
    }

    {
        QBitArray input(30);
        input.clearBit(0);
        input.clearBit(1);
        input.clearBit(2);
        input.clearBit(3);

        input.setBit(4);
        input.setBit(5);
        input.clearBit(6);
        input.clearBit(7);

        input.setBit(8);
        input.clearBit(9);
        input.clearBit(10);
        input.clearBit(11);

        input.clearBit(12);
        input.clearBit(13);
        input.setBit(14);
        input.clearBit(15);

        const QJsonObject expectedResult
        {
            { "bit_count",    30 },
            { "encoded_bits", "MEEAAA==" }
        };
        QTest::newRow("non-empty") << input << QJsonValue(expectedResult);
    }
}

// Test: serialize<std::string>() method -----------------------------------------------------------

Q_DECLARE_METATYPE(std::string)

void TestSerialization::testSerializeStdString()
{
    QFETCH(std::string, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeStdString_data()
{
    QTest::addColumn<std::string>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    QTest::newRow("empty") << std::string() << QJsonValue("");
    QTest::newRow("a z A Z 0 9 + - ?") << std::string("a z A Z 0 9 + - ?")
                                       << QJsonValue("a z A Z 0 9 + - ?");
}

// Test: serialize<std::wstring>() method ----------------------------------------------------------

Q_DECLARE_METATYPE(std::wstring)

void TestSerialization::testSerializeStdWString()
{
    QFETCH(std::wstring, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeStdWString_data()
{
    QTest::addColumn<std::wstring>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    QTest::newRow("empty") << std::wstring() << QJsonValue("");
    QTest::newRow("a z A Z 0 9 + - ?") << std::wstring(L"a z A Z 0 9 + - ?")
                                       << QJsonValue("a z A Z 0 9 + - ?");
}

// Test: serialize<std::u16string>() method --------------------------------------------------------

Q_DECLARE_METATYPE(std::u16string)

void TestSerialization::testSerializeStdU16String()
{
    QFETCH(std::u16string, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeStdU16String_data()
{
    QTest::addColumn<std::u16string>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    QTest::newRow("empty") << std::u16string() << QJsonValue("");
    QTest::newRow("a z A Z 0 9 + - ?") << std::u16string(u"a z A Z 0 9 + - ?")
                                       << QJsonValue("a z A Z 0 9 + - ?");
}

// Test: serialize<std::u32string>() method --------------------------------------------------------

Q_DECLARE_METATYPE(std::u32string)

void TestSerialization::testSerializeStdU32String()
{
    QFETCH(std::u32string, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeStdU32String_data()
{
    QTest::addColumn<std::u32string>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    QTest::newRow("empty") << std::u32string() << QJsonValue("");
    QTest::newRow("a z A Z 0 9 + - ?") << std::u32string(U"a z A Z 0 9 + - ?")
                                       << QJsonValue("a z A Z 0 9 + - ?");
}

// Test: serialize<QDate>() method -----------------------------------------------------------------

void TestSerialization::testSerializeQDate()
{
    QFETCH(QDate, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeQDate_data()
{
    QTest::addColumn<QDate>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    QTest::newRow("empty") << QDate() << QJsonValue("");
    QTest::newRow("2020-08-05") << QDate(2020, 8, 5) << QJsonValue("2020-08-05");
}

// Test: serialize<QTime>() method -----------------------------------------------------------------

void TestSerialization::testSerializeQTime()
{
    QFETCH(QTime, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeQTime_data()
{
    QTest::addColumn<QTime>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    QTest::newRow("empty") << QTime() << QJsonValue("");
    QTest::newRow("12:34:56.789") << QTime(12, 34, 56, 789) << QJsonValue("12:34:56.789");
}

// Test: serialize<QDateTime>() method -------------------------------------------------------------

void TestSerialization::testSerializeQDateTime()
{
    QFETCH(QDateTime, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeQDateTime_data()
{
    QTest::addColumn<QDateTime>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    QTest::newRow("empty") << QDateTime() << QJsonValue("");
    QTest::newRow("2020-08-05T12:34:56.789Z")
            << QDateTime(QDate(2020, 8, 5), QTime(12, 34, 56, 789), Qt::UTC)
            << QJsonValue("2020-08-05T12:34:56.789Z");
}

// Test: serialize<QVariant>() method --------------------------------------------------------------

void TestSerialization::testSerializeQVariant()
{
    QFETCH(QVariant, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeQVariant_data()
{
    QTest::addColumn<QVariant>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    QTest::newRow("Nullptr") << QVariant::fromValue(nullptr) << QJsonValue(QJsonValue::Null);

    QTest::newRow("Bool: true")  << QVariant::fromValue(true)  << QJsonValue(true);
    QTest::newRow("Bool: false") << QVariant::fromValue(false) << QJsonValue(false);

    QTest::newRow("SChar: -1") << QVariant::fromValue<signed char>(-1) << QJsonValue(-1);
    QTest::newRow("SChar:  0") << QVariant::fromValue<signed char>( 0) << QJsonValue( 0);
    QTest::newRow("SChar:  1") << QVariant::fromValue<signed char>( 1) << QJsonValue( 1);

    QTest::newRow("UChar: 0") << QVariant::fromValue<unsigned char>(0U) << QJsonValue(0);
    QTest::newRow("UChar: 1") << QVariant::fromValue<unsigned char>(1U) << QJsonValue(1);

    QTest::newRow("Short: -1") << QVariant::fromValue<short>(-1) << QJsonValue(-1);
    QTest::newRow("Short:  0") << QVariant::fromValue<short>( 0) << QJsonValue( 0);
    QTest::newRow("Short:  1") << QVariant::fromValue<short>( 1) << QJsonValue( 1);

    QTest::newRow("UShort: 0") << QVariant::fromValue<unsigned short>(0U) << QJsonValue(0);
    QTest::newRow("UShort: 1") << QVariant::fromValue<unsigned short>(1U) << QJsonValue(1);

    QTest::newRow("Int: -1") << QVariant::fromValue<int>(-1) << QJsonValue(-1);
    QTest::newRow("Int:  0") << QVariant::fromValue<int>( 0) << QJsonValue( 0);
    QTest::newRow("Int:  1") << QVariant::fromValue<int>( 1) << QJsonValue( 1);

    QTest::newRow("UInt: 0") << QVariant::fromValue<unsigned int>(0U) << QJsonValue(0);
    QTest::newRow("UInt: 1") << QVariant::fromValue<unsigned int>(1U) << QJsonValue(1);

    QTest::newRow("Long: -1") << QVariant::fromValue<long>(-1L) << QJsonValue(-1);
    QTest::newRow("Long:  0") << QVariant::fromValue<long>( 0L) << QJsonValue( 0);
    QTest::newRow("Long:  1") << QVariant::fromValue<long>( 1L) << QJsonValue( 1);

    QTest::newRow("ULong: 0") << QVariant::fromValue<unsigned long>(0UL) << QJsonValue(0);
    QTest::newRow("ULong: 1") << QVariant::fromValue<unsigned long>(1UL) << QJsonValue(1);

    QTest::newRow("LongLong: min")
            << QVariant::fromValue(std::numeric_limits<long long>::min())
            << QJsonValue(QString::number(std::numeric_limits<long long>::min()));
    QTest::newRow("LongLong: -1") << QVariant::fromValue<long long>(-1LL) << QJsonValue(-1);
    QTest::newRow("LongLong:  0") << QVariant::fromValue<long long>( 0LL) << QJsonValue( 0);
    QTest::newRow("LongLong:  1") << QVariant::fromValue<long long>( 1LL) << QJsonValue( 1);
    QTest::newRow("LongLong: max")
            << QVariant::fromValue(std::numeric_limits<long long>::max())
            << QJsonValue(QString::number(std::numeric_limits<long long>::max()));

    QTest::newRow("ULongLong: 0") << QVariant::fromValue<unsigned long long>(0ULL) << QJsonValue(0);
    QTest::newRow("ULongLong: 1") << QVariant::fromValue<unsigned long long>(1ULL) << QJsonValue(1);
    QTest::newRow("ULongLong: max")
            << QVariant::fromValue(std::numeric_limits<unsigned long long>::max())
            << QJsonValue(QString::number(std::numeric_limits<unsigned long long>::max()));

    QTest::newRow("Float: -1") << QVariant::fromValue<float>(-1.0F) << QJsonValue(-1.0);
    QTest::newRow("Float:  0") << QVariant::fromValue<float>( 0.0F) << QJsonValue( 0.0);
    QTest::newRow("Float:  1") << QVariant::fromValue<float>( 1.0F) << QJsonValue( 1.0);

    QTest::newRow("Double: min") << QVariant::fromValue(std::numeric_limits<double>::lowest())
                                 << QJsonValue(std::numeric_limits<double>::lowest());
    QTest::newRow("Double: -1") << QVariant::fromValue<double>(-1.0) << QJsonValue(-1.0);
    QTest::newRow("Double:  0") << QVariant::fromValue<double>( 0.0) << QJsonValue( 0.0);
    QTest::newRow("Double:  1") << QVariant::fromValue<double>( 1.0) << QJsonValue( 1.0);
    QTest::newRow("Double: max") << QVariant::fromValue(std::numeric_limits<double>::max())
                                 << QJsonValue(std::numeric_limits<double>::max());

    QTest::newRow("QTime") << QVariant(QTime(12, 55, 30, 123)) << QJsonValue("12:55:30.123");
    QTest::newRow("QDate") << QVariant(QDate(2020, 8, 15)) << QJsonValue("2020-08-15");
    QTest::newRow("QDateTime")
            << QVariant(QDateTime(QDate(2020, 8, 15), QTime(12, 55, 30, 123), Qt::UTC))
            << QJsonValue("2020-08-15T12:55:30.123Z");

    QTest::newRow("Char") << QVariant::fromValue<char>('X') << QJsonValue("X");
    QTest::newRow("QChar") << QVariant(QChar('y')) << QJsonValue("y");
    QTest::newRow("QString") << QVariant(QString("abc123")) << QJsonValue("abc123");
    QTest::newRow("QByteArray") << QVariant(QByteArray("a z A Z 0 9 + - ?"))
                                << QJsonValue("YSB6IEEgWiAwIDkgKyAtID8=");

    {
        QBitArray input(30);
        input.clearBit(0);
        input.clearBit(1);
        input.clearBit(2);
        input.clearBit(3);

        input.setBit(4);
        input.setBit(5);
        input.clearBit(6);
        input.clearBit(7);

        input.setBit(8);
        input.clearBit(9);
        input.clearBit(10);
        input.clearBit(11);

        input.clearBit(12);
        input.clearBit(13);
        input.setBit(14);
        input.clearBit(15);

        const QJsonObject expectedResult
        {
            { "bit_count",    30 },
            { "encoded_bits", "MEEAAA==" }
        };
        QTest::newRow("QBitArray") << QVariant(input) << QJsonValue(expectedResult);
    }

    QTest::newRow("QUrl") << QVariant(QUrl("http://www.example.com"))
                          << QJsonValue("http://www.example.com");
    QTest::newRow("QUuid") << QVariant(QUuid("{01234567-89ab-cdef-0123-456789abcdef}"))
                           << QJsonValue("{01234567-89ab-cdef-0123-456789abcdef}");
    QTest::newRow("QLocale")
            << QVariant(QLocale(QLocale::English, QLocale::LatinScript, QLocale::UnitedKingdom))
            << QJsonValue("en-GB");

    {
        const QJsonObject expectedResult
        {
            { "pattern",        "abc*" },
            { "case_sensitive", true },
            { "syntax",         "Wildcard" }
        };
        QTest::newRow("QRegExp") << QVariant(QRegExp("abc*", Qt::CaseSensitive, QRegExp::Wildcard))
                                 << QJsonValue(expectedResult);
    }

    {
        const QJsonObject expectedResult
        {
            { "pattern", "[a-z0-9]+" },
            {
                "options",
                QJsonArray(
                { "CaseInsensitive", "ExtendedPatternSyntax", "InvertedGreediness", "DontCapture" })
            }
        };
        QTest::newRow("QRegularExpression")
                << QVariant(QRegularExpression("[a-z0-9]+",
                                               QRegularExpression::CaseInsensitiveOption |
                                               QRegularExpression::ExtendedPatternSyntaxOption |
                                               QRegularExpression::InvertedGreedinessOption |
                                               QRegularExpression::DontCaptureOption))
                << QJsonValue(expectedResult);
    }

    {
        const QJsonObject expectedResult
        {
            { "width",  123 },
            { "height", 456 }
        };
        QTest::newRow("QSize") << QVariant(QSize(123, 456)) << QJsonValue(expectedResult);
    }

    {
        const QJsonObject expectedResult
        {
            { "width",  123.4 },
            { "height", 567.8 }
        };
        QTest::newRow("QSize") << QVariant(QSizeF(123.4, 567.8)) << QJsonValue(expectedResult);
    }

    {
        const QJsonObject expectedResult
        {
            { "x", 123 },
            { "y", 456 }
        };
        QTest::newRow("QPoint") << QVariant(QPoint(123, 456)) << QJsonValue(expectedResult);
    }

    {
        const QJsonObject expectedResult
        {
            { "x", 123.4 },
            { "y", 567.8 }
        };
        QTest::newRow("QPointF") << QVariant(QPointF(123.4, 567.8)) << QJsonValue(expectedResult);
    }

    {
        const QJsonObject expectedResult
        {
            { "x1", 123 },
            { "y1", 456 },
            { "x2", 777 },
            { "y2", 888 }
        };
        QTest::newRow("QLine") << QVariant(QLine(123, 456, 777, 888)) << QJsonValue(expectedResult);
    }

    {
        const QJsonObject expectedResult
        {
            { "x1", 123.4 },
            { "y1", 567.8 },
            { "x2", 111.1 },
            { "y2", 222.2 }
        };
        QTest::newRow("QLineF") << QVariant(QLineF(123.4, 567.8, 111.1, 222.2))
                                << QJsonValue(expectedResult);
    }

    {
        const QJsonObject expectedResult
        {
            { "x",      123 },
            { "y",      456 },
            { "width",  777 },
            { "height", 888 }
        };
        QTest::newRow("QRect") << QVariant(QRect(123, 456, 777, 888)) << QJsonValue(expectedResult);
    }

    {
        const QJsonObject expectedResult
        {
            { "x",      123.4 },
            { "y",      567.8 },
            { "width",  111.1 },
            { "height", 222.2 }
        };
        QTest::newRow("QRectF") << QVariant(QRectF(123.4, 567.8, 111.1, 222.2))
                                << QJsonValue(expectedResult);
    }

    {
        const QStringList input { "a", "b", "C", "D", "1" };
        QTest::newRow("QStringList") << QVariant(input)
                                     << QJsonValue(QJsonArray::fromStringList(input));
    }

    {
        const QByteArrayList input { "a", "b", "C" };
        const QJsonArray expectedResult
        {
            QString::fromLatin1(QByteArray("a").toBase64()),
                    QString::fromLatin1(QByteArray("b").toBase64()),
                    QString::fromLatin1(QByteArray("C").toBase64())
        };
        QTest::newRow("QByteArrayList") << QVariant::fromValue(input) << QJsonValue(expectedResult);
    }

    {
        const QVariantList input { "a", 1, false };
        const QJsonArray expectedResult { "a", 1, false };
        QTest::newRow("QVariantList") << QVariant::fromValue(input) << QJsonValue(expectedResult);
    }

    {
        const QVariantMap input
        {
            { "string", "asd" },
            { "bool",   false },
            { "double", 123.4 }
        };
        const QJsonObject expectedResult
        {
            { "string", "asd" },
            { "bool",   false },
            { "double", 123.4 }
        };
        QTest::newRow("QVariantMap") << QVariant::fromValue(input) << QJsonValue(expectedResult);
    }

    {
        const QVariantHash input
        {
            { "string", "asd" },
            { "bool",   false },
            { "double", 123.4 }
        };
        const QJsonObject expectedResult
        {
            { "string", "asd" },
            { "bool",   false },
            { "double", 123.4 }
        };
        QTest::newRow("QVariantHash") << QVariant::fromValue(input) << QJsonValue(expectedResult);
    }

    QTest::newRow("QJsonValue") << QVariant(QJsonValue("abc123")) << QJsonValue("abc123");

    {
        const QJsonArray value
        {
            QJsonValue(), false, 123.4
        };
        QTest::newRow("QJsonArray") << QVariant(value) << QJsonValue(value);
    }

    {
        const QJsonObject value
        {
            { "null",   QJsonValue() },
            { "bool",   false },
            { "double", 123.4 }
        };
        QTest::newRow("QJsonObject") << QVariant(value) << QJsonValue(value);
    }

    {
        const QJsonArray value
        {
            QJsonValue(), false, 123.4
        };
        QTest::newRow("QJsonDocument") << QVariant(QJsonDocument(value)) << QJsonValue(value);
    }

    QTest::newRow("QCborValue") << QVariant::fromValue(QCborValue("abc123"))
                                << QJsonValue("abc123");

    {
        const QCborArray value
        {
            QCborValue(nullptr), false, 123.4
        };
        QTest::newRow("QCborArray") << QVariant::fromValue(value)
                                    << QJsonValue(value.toJsonArray());
    }

    {
        const QCborMap value
        {
            { "null",   QCborValue(nullptr) },
            { "bool",   false },
            { "double", 123.4 }
        };
        QTest::newRow("QCborMap") << QVariant::fromValue(value) << QJsonValue(value.toJsonObject());
    }

    QTest::newRow("QCborSimpleType") << QVariant::fromValue(QCborSimpleType::True)
                                     << QJsonValue(true);

    QTest::newRow("invalid") << QVariant() << QJsonValue(QJsonValue::Undefined);
}

// Test: serialize<QUrl>() method ------------------------------------------------------------------

void TestSerialization::testSerializeQUrl()
{
    QFETCH(QUrl, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeQUrl_data()
{
    QTest::addColumn<QUrl>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    QTest::newRow("empty") << QUrl() << QJsonValue("");
    QTest::newRow("http://www.example.com") << QUrl("http://www.example.com")
                                            << QJsonValue("http://www.example.com");
}

// Test: serialize<QUuid>() method -----------------------------------------------------------------

void TestSerialization::testSerializeQUuid()
{
    QFETCH(QUuid, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeQUuid_data()
{
    QTest::addColumn<QUuid>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    QTest::newRow("empty") << QUuid() << QJsonValue("{00000000-0000-0000-0000-000000000000}");
    QTest::newRow("{01234567-89ab-cdef-0123-456789abcdef}")
            << QUuid("{01234567-89ab-cdef-0123-456789abcdef}")
            << QJsonValue("{01234567-89ab-cdef-0123-456789abcdef}");
}

// Test: serialize<QLocale>() method ---------------------------------------------------------------

void TestSerialization::testSerializeQLocale()
{
    QFETCH(QLocale, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeQLocale_data()
{
    QTest::addColumn<QLocale>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    QTest::newRow("C") << QLocale::c() << QJsonValue("en");
    QTest::newRow("en-GB")
            << QLocale(QLocale::English, QLocale::LatinScript, QLocale::UnitedKingdom)
            << QJsonValue("en-GB");
    QTest::newRow("de-AT") << QLocale(QLocale::German, QLocale::Austria) << QJsonValue("de-AT");
    QTest::newRow("ru") << QLocale(QLocale::Russian, QLocale::Russia) << QJsonValue("ru");
}

// Test: serialize<QRegExp>() method ---------------------------------------------------------------

void TestSerialization::testSerializeQRegExp()
{
    QFETCH(QRegExp, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeQRegExp_data()
{
    QTest::addColumn<QRegExp>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    {
        const QJsonObject expectedResult
        {
            { "pattern",        "" },
            { "case_sensitive", true },
            { "syntax",         "RegExp" }
        };
        QTest::newRow("empty") << QRegExp() << QJsonValue(expectedResult);
    }

    {
        const QJsonObject expectedResult
        {
            { "pattern",        "[a-z0-9]+" },
            { "case_sensitive", false },
            { "syntax",         "RegExp2" }
        };
        QTest::newRow("with pattern") << QRegExp("[a-z0-9]+", Qt::CaseInsensitive, QRegExp::RegExp2)
                                      << QJsonValue(expectedResult);
    }
}

// Test: serialize<QRegularExpression>() method ----------------------------------------------------

void TestSerialization::testSerializeQRegularExpression()
{
    QFETCH(QRegularExpression, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeQRegularExpression_data()
{
    QTest::addColumn<QRegularExpression>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    {
        const QJsonObject expectedResult
        {
            { "pattern", "" },
            { "options", QJsonArray() }
        };
        QTest::newRow("empty") << QRegularExpression() << QJsonValue(expectedResult);
    }

    {
        const QJsonObject expectedResult
        {
            { "pattern", "[a-z0-9]+" },
            { "options", QJsonArray({ "DotMatchesEverything", "Multiline" }) }
        };
        QTest::newRow("with pattern")
                << QRegularExpression("[a-z0-9]+",
                                      QRegularExpression::DotMatchesEverythingOption |
                                      QRegularExpression::MultilineOption)
                << QJsonValue(expectedResult);
    }
}

// Test: serialize<QSize>() method -----------------------------------------------------------------

void TestSerialization::testSerializeQSize()
{
    QFETCH(QSize, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeQSize_data()
{
    QTest::addColumn<QSize>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    {
        const QJsonObject expectedResult
        {
            { "width",  -1 },
            { "height", -1 }
        };
        QTest::newRow("empty") << QSize() << QJsonValue(expectedResult);
    }

    {
        const QJsonObject expectedResult
        {
            { "width",  123 },
            { "height", 456 }
        };
        QTest::newRow("123 x 456") << QSize(123, 456) << QJsonValue(expectedResult);
    }
}

// Test: serialize<QSizeF>() method ----------------------------------------------------------------

void TestSerialization::testSerializeQSizeF()
{
    QFETCH(QSizeF, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeQSizeF_data()
{
    QTest::addColumn<QSizeF>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    {
        const QJsonObject expectedResult
        {
            { "width",  -1.0 },
            { "height", -1.0 }
        };
        QTest::newRow("empty") << QSizeF() << QJsonValue(expectedResult);
    }

    {
        const QJsonObject expectedResult
        {
            { "width",  123.4 },
            { "height", 567.8 }
        };
        QTest::newRow("123.4 x 567.8") << QSizeF(123.4, 567.8) << QJsonValue(expectedResult);
    }
}

// Test: serialize<QPoint>() method ----------------------------------------------------------------

void TestSerialization::testSerializeQPoint()
{
    QFETCH(QPoint, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeQPoint_data()
{
    QTest::addColumn<QPoint>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    {
        const QJsonObject expectedResult
        {
            { "x", 0 },
            { "y", 0 }
        };
        QTest::newRow("empty") << QPoint() << QJsonValue(expectedResult);
    }

    {
        const QJsonObject expectedResult
        {
            { "x", 123 },
            { "y", 456 }
        };
        QTest::newRow("123 x 456") << QPoint(123, 456) << QJsonValue(expectedResult);
    }
}

// Test: serialize<QPointF>() method ---------------------------------------------------------------

void TestSerialization::testSerializeQPointF()
{
    QFETCH(QPointF, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeQPointF_data()
{
    QTest::addColumn<QPointF>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    {
        const QJsonObject expectedResult
        {
            { "x", 0.0 },
            { "y", 0.0 }
        };
        QTest::newRow("empty") << QPointF() << QJsonValue(expectedResult);
    }

    {
        const QJsonObject expectedResult
        {
            { "x", 123.4 },
            { "y", 567.8 }
        };
        QTest::newRow("123.4 x 567.8") << QPointF(123.4, 567.8) << QJsonValue(expectedResult);
    }
}

// Test: serialize<QLine>() method -----------------------------------------------------------------

void TestSerialization::testSerializeQLine()
{
    QFETCH(QLine, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeQLine_data()
{
    QTest::addColumn<QLine>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    {
        const QJsonObject expectedResult
        {
            { "x1", 0 },
            { "y1", 0 },
            { "x2", 0 },
            { "y2", 0 }
        };
        QTest::newRow("empty") << QLine() << QJsonValue(expectedResult);
    }

    {
        const QJsonObject expectedResult
        {
            { "x1", 123 },
            { "y1", 456 },
            { "x2", 777 },
            { "y2", 888 }
        };
        QTest::newRow("123 x 456, 777 x 888") << QLine(123, 456, 777, 888)
                                              << QJsonValue(expectedResult);
    }
}

// Test: serialize<QLineF>() method ----------------------------------------------------------------

void TestSerialization::testSerializeQLineF()
{
    QFETCH(QLineF, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeQLineF_data()
{
    QTest::addColumn<QLineF>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    {
        const QJsonObject expectedResult
        {
            { "x1", 0.0 },
            { "y1", 0.0 },
            { "x2", 0.0 },
            { "y2", 0.0 }
        };
        QTest::newRow("empty") << QLineF() << QJsonValue(expectedResult);
    }

    {
        const QJsonObject expectedResult
        {
            { "x1", 123.4 },
            { "y1", 567.8 },
            { "x2", 111.1 },
            { "y2", 222.2 }
        };
        QTest::newRow("123.4 x 567.8, 111.1 x 222.2") << QLineF(123.4, 567.8, 111.1, 222.2)
                                                      << QJsonValue(expectedResult);
    }
}

// Test: serialize<QRect>() method -----------------------------------------------------------------

void TestSerialization::testSerializeQRect()
{
    QFETCH(QRect, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeQRect_data()
{
    QTest::addColumn<QRect>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    {
        const QJsonObject expectedResult
        {
            { "x",      0 },
            { "y",      0 },
            { "width",  0 },
            { "height", 0 }
        };
        QTest::newRow("empty") << QRect() << QJsonValue(expectedResult);
    }

    {
        const QJsonObject expectedResult
        {
            { "x",      123 },
            { "y",      456 },
            { "width",  777 },
            { "height", 888 }
        };
        QTest::newRow("123 x 456, 777 x 888") << QRect(123, 456, 777, 888)
                                              << QJsonValue(expectedResult);
    }
}

// Test: serialize<QRectF>() method ----------------------------------------------------------------

void TestSerialization::testSerializeQRectF()
{
    QFETCH(QRectF, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeQRectF_data()
{
    QTest::addColumn<QRectF>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    {
        const QJsonObject expectedResult
        {
            { "x",      0.0 },
            { "y",      0.0 },
            { "width",  0.0 },
            { "height", 0.0 }
        };
        QTest::newRow("empty") << QRectF() << QJsonValue(expectedResult);
    }

    {
        const QJsonObject expectedResult
        {
            { "x",      123.4 },
            { "y",      567.8 },
            { "width",  111.1 },
            { "height", 222.2 }
        };
        QTest::newRow("123.4 x 567.8, 111.1 x 222.2") << QRectF(123.4, 567.8, 111.1, 222.2)
                                                      << QJsonValue(expectedResult);
    }
}

// Test: serialize<QStringList>() method -----------------------------------------------------------

void TestSerialization::testSerializeQStringList()
{
    QFETCH(QStringList, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeQStringList_data()
{
    QTest::addColumn<QStringList>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    QTest::newRow("empty") << QStringList() << QJsonValue(QJsonArray());

    {
        const QStringList input { "a", "b", "C", "D", "1" };
        QTest::newRow("non-empty") << input << QJsonValue(QJsonArray::fromStringList(input));
    }
}

// Test: serialize<QJsonValue>() method ------------------------------------------------------------

void TestSerialization::testSerializeQJsonValue()
{
    QFETCH(QJsonValue, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeQJsonValue_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    QTest::newRow("null")   << QJsonValue()         << QJsonValue();
    QTest::newRow("bool")   << QJsonValue(true)     << QJsonValue(true);
    QTest::newRow("double") << QJsonValue(123.4)    << QJsonValue(123.4);
    QTest::newRow("string") << QJsonValue("abc123") << QJsonValue("abc123");

    {
        const QJsonArray value
        {
            QJsonValue(), false, 123.4
        };
        QTest::newRow("array") << QJsonValue(value) << QJsonValue(value);
    }

    {
        const QJsonObject value
        {
            { "null",   QJsonValue() },
            { "bool",   false },
            { "double", 123.4 }
        };
        QTest::newRow("object") << QJsonValue(value) << QJsonValue(value);
    }
}

// Test: serialize<QJsonArray>() method ------------------------------------------------------------

void TestSerialization::testSerializeQJsonArray()
{
    QFETCH(QJsonArray, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeQJsonArray_data()
{
    QTest::addColumn<QJsonArray>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    QTest::newRow("empty") << QJsonArray() << QJsonValue(QJsonArray());

    {
        const QJsonArray value
        {
            QJsonValue(), false, 123.4
        };
        QTest::newRow("non-empty") << value << QJsonValue(value);
    }
}

// Test: serialize<QJsonObject>() method -----------------------------------------------------------

void TestSerialization::testSerializeQJsonObject()
{
    QFETCH(QJsonObject, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeQJsonObject_data()
{
    QTest::addColumn<QJsonObject>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    QTest::newRow("empty") << QJsonObject() << QJsonValue(QJsonObject());

    {
        const QJsonObject value
        {
            { "null",   QJsonValue() },
            { "bool",   false },
            { "double", 123.4 }
        };
        QTest::newRow("non-empty") << value << QJsonValue(value);
    }
}

// Test: serialize<QJsonDocument>() method ---------------------------------------------------------

void TestSerialization::testSerializeQJsonDocument()
{
    QFETCH(QJsonDocument, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeQJsonDocument_data()
{
    QTest::addColumn<QJsonDocument>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    QTest::newRow("empty") << QJsonDocument() << QJsonValue();

    {
        const QJsonArray value
        {
            QJsonValue(), false, 123.4
        };
        QTest::newRow("array") << QJsonDocument(value) << QJsonValue(value);
    }

    {
        const QJsonObject value
        {
            { "null",   QJsonValue() },
            { "bool",   false },
            { "double", 123.4 }
        };
        QTest::newRow("object") << QJsonDocument(value) << QJsonValue(value);
    }
}

// Test: serialize<QCborValue>() method ------------------------------------------------------------

void TestSerialization::testSerializeQCborValue()
{
    QFETCH(QCborValue, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeQCborValue_data()
{
    QTest::addColumn<QCborValue>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    QTest::newRow("null")   << QCborValue(nullptr)  << QJsonValue();
    QTest::newRow("bool")   << QCborValue(true)     << QJsonValue(true);
    QTest::newRow("int")    << QCborValue(100)      << QJsonValue(100);
    QTest::newRow("double") << QCborValue(123.4)    << QJsonValue(123.4);
    QTest::newRow("string") << QCborValue("abc123") << QJsonValue("abc123");

    {
        const QCborArray value
        {
            QCborValue(nullptr), false, 123.4
        };
        QTest::newRow("array") << QCborValue(value) << QJsonValue(value.toJsonArray());
    }

    {
        const QCborMap value
        {
            { "null",   QCborValue(nullptr) },
            { "bool",   false },
            { "double", 123.4 }
        };
        QTest::newRow("object") << QCborValue(value) << QJsonValue(value.toJsonObject());
    }
}

// Test: serialize<QCborArray>() method ------------------------------------------------------------

void TestSerialization::testSerializeQCborArray()
{
    QFETCH(QCborArray, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeQCborArray_data()
{
    QTest::addColumn<QCborArray>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    QTest::newRow("empty") << QCborArray() << QJsonValue(QJsonArray());

    {
        const QCborArray value
        {
            QCborValue(nullptr), false, 123.4
        };
        QTest::newRow("non-empty") << value << QJsonValue(value.toJsonArray());
    }
}

// Test: serialize<QCborMap>() method --------------------------------------------------------------

void TestSerialization::testSerializeQCborMap()
{
    QFETCH(QCborMap, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeQCborMap_data()
{
    QTest::addColumn<QCborMap>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    QTest::newRow("empty") << QCborMap() << QJsonValue(QJsonObject());

    {
        const QCborMap value
        {
            { "null",   QCborValue(nullptr) },
            { "bool",   false },
            { "double", 123.4 }
        };
        QTest::newRow("non-empty") << value << QJsonValue(value.toJsonObject());
    }
}

// Test: serialize<QCborSimpleType>() method -------------------------------------------------------

void TestSerialization::testSerializeQCborSimpleType()
{
    QFETCH(QCborSimpleType, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeQCborSimpleType_data()
{
    QTest::addColumn<QCborSimpleType>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    QTest::newRow("true")       << QCborSimpleType::True       << QJsonValue(true);
    QTest::newRow("false")      << QCborSimpleType::False      << QJsonValue(false);
    QTest::newRow("null")       << QCborSimpleType::Null       << QJsonValue();
    QTest::newRow("undefined")  << QCborSimpleType::Undefined  << QJsonValue(QJsonValue::Undefined);
}

// Test: serialize<QPair>() method -----------------------------------------------------------------

using QPairStringInt = QPair<QString, int>;

void TestSerialization::testSerializeQPair()
{
    QFETCH(QPairStringInt, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeQPair_data()
{
    QTest::addColumn<QPairStringInt>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    {
        const QJsonObject value
        {
            { "first",  "asd" },
            { "second", 123 },
        };
        QTest::newRow("1") << QPairStringInt("asd", 123) << QJsonValue(value);
    }

    {
        const QJsonObject value
        {
            { "first",  "abc" },
            { "second", 456 },
        };
        QTest::newRow("2") << QPairStringInt("abc", 456) << QJsonValue(value);
    }
}

// Test: serialize<std::pair>() method -----------------------------------------------------------------

using StdPairStringInt = std::pair<QString, int>;

void TestSerialization::testSerializeStdPair()
{
    QFETCH(StdPairStringInt, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeStdPair_data()
{
    QTest::addColumn<StdPairStringInt>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    {
        const QJsonObject value
        {
            { "first",  "asd" },
            { "second", 123 },
        };
        QTest::newRow("1") << StdPairStringInt("asd", 123) << QJsonValue(value);
    }

    {
        const QJsonObject value
        {
            { "first",  "abc" },
            { "second", 456 },
        };
        QTest::newRow("2") << StdPairStringInt("abc", 456) << QJsonValue(value);
    }
}

// Test: serialize<QList>() method -----------------------------------------------------------

void TestSerialization::testSerializeQList()
{
    QFETCH(QList<int>, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeQList_data()
{
    QTest::addColumn<QList<int>>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    QTest::newRow("empty") << QList<int>() << QJsonValue(QJsonArray());

    {
        const QList<int> input { -1, 0, 1 };
        QTest::newRow("non-empty") << input << QJsonValue(QJsonArray { -1, 0, 1 } );
    }
}

// Test: serialize<std::list>() method -----------------------------------------------------------

void TestSerialization::testSerializeStdList()
{
    QFETCH(std::list<int>, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeStdList_data()
{
    QTest::addColumn<std::list<int>>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    QTest::newRow("empty") << std::list<int>() << QJsonValue(QJsonArray());

    {
        const std::list<int> input { -1, 0, 1 };
        QTest::newRow("non-empty") << input << QJsonValue(QJsonArray { -1, 0, 1 } );
    }
}

// Test: serialize<QVector>() method -----------------------------------------------------------

void TestSerialization::testSerializeQVector()
{
    QFETCH(QVector<int>, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeQVector_data()
{
    QTest::addColumn<QVector<int>>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    QTest::newRow("empty") << QVector<int>() << QJsonValue(QJsonArray());

    {
        const QVector<int> input { -1, 0, 1 };
        QTest::newRow("non-empty") << input << QJsonValue(QJsonArray { -1, 0, 1 } );
    }
}

// Test: serialize<std::vector>() method -----------------------------------------------------------

void TestSerialization::testSerializeStdVector()
{
    QFETCH(std::vector<int>, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeStdVector_data()
{
    QTest::addColumn<std::vector<int>>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    QTest::newRow("empty") << std::vector<int>() << QJsonValue(QJsonArray());

    {
        const std::vector<int> input { -1, 0, 1 };
        QTest::newRow("non-empty") << input << QJsonValue(QJsonArray { -1, 0, 1 } );
    }
}

// Test: serialize<QSet>() method -----------------------------------------------------------

void TestSerialization::testSerializeQSet()
{
    QFETCH(QSet<int>, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QVERIFY(result.isArray());
    QVERIFY(expectedResult.isArray());

    const auto resultArray = result.toArray();
    const auto expectedResultArray = expectedResult.toArray();

    QCOMPARE(resultArray.size(), expectedResultArray.size());

    for (const auto &resultItem : resultArray)
    {
        QVERIFY(expectedResultArray.contains(resultItem));
    }
}

void TestSerialization::testSerializeQSet_data()
{
    QTest::addColumn<QSet<int>>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    QTest::newRow("empty") << QSet<int>() << QJsonValue(QJsonArray());

    {
        const QSet<int> input { -1, 0, 1 };
        QTest::newRow("non-empty") << input << QJsonValue(QJsonArray { -1, 0, 1 } );
    }
}

// Test: serialize<QMap>() method -----------------------------------------------------------

using QMapIntString = QMap<int, QString>;

void TestSerialization::testSerializeQMap()
{
    QFETCH(QMapIntString, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeQMap_data()
{
    QTest::addColumn<QMapIntString>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    QTest::newRow("empty") << QMapIntString() << QJsonValue(QJsonObject());

    {
        const QMapIntString input
        {
            { -1, "a" },
            {  0, "b" },
            {  1, "c" }
        };
        const QJsonObject expectedResult
        {
            { "-1", "a" },
            {  "0", "b" },
            {  "1", "c" }
        };
        QTest::newRow("non-empty") << input << QJsonValue(expectedResult);
    }
}

// Test: serialize<std::map>() method -----------------------------------------------------------

using StdMapIntString = std::map<int, QString>;

void TestSerialization::testSerializeStdMap()
{
    QFETCH(StdMapIntString, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeStdMap_data()
{
    QTest::addColumn<StdMapIntString>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    QTest::newRow("empty") << StdMapIntString() << QJsonValue(QJsonObject());

    {
        const StdMapIntString input
        {
            { -1, "a" },
            {  0, "b" },
            {  1, "c" }
        };
        const QJsonObject expectedResult
        {
            { "-1", "a" },
            {  "0", "b" },
            {  "1", "c" }
        };
        QTest::newRow("non-empty") << input << QJsonValue(expectedResult);
    }
}

// Test: serialize<QHash>() method -----------------------------------------------------------

using QHashQDateString = QHash<QDate, QString>;

void TestSerialization::testSerializeQHash()
{
    QFETCH(QHashQDateString, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeQHash_data()
{
    QTest::addColumn<QHashQDateString>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    QTest::newRow("empty") << QHashQDateString() << QJsonValue(QJsonObject());

    {
        const QHashQDateString input
        {
            { QDate(2020, 1, 1), "a" },
            { QDate(2020, 1, 2), "b" },
            { QDate(2020, 1, 3), "c" }
        };
        const QJsonObject expectedResult
        {
            { "2020-01-01", "a" },
            { "2020-01-02", "b" },
            { "2020-01-03", "c" }
        };
        QTest::newRow("non-empty") << input << QJsonValue(expectedResult);
    }
}

// Test: serialize<QHash>() method -----------------------------------------------------------

using StdUnorderedMapIntString = std::unordered_map<int, QString>;
Q_DECLARE_METATYPE(StdUnorderedMapIntString)

void TestSerialization::testSerializeStdUnorderedMap()
{
    QFETCH(StdUnorderedMapIntString, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeStdUnorderedMap_data()
{
    QTest::addColumn<StdUnorderedMapIntString>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    QTest::newRow("empty") << StdUnorderedMapIntString() << QJsonValue(QJsonObject());

    {
        const StdUnorderedMapIntString input
        {
            { -1, "a" },
            {  0, "b" },
            {  1, "c" }
        };
        const QJsonObject expectedResult
        {
            { "-1", "a" },
            {  "0", "b" },
            {  "1", "c" }
        };
        QTest::newRow("non-empty") << input << QJsonValue(expectedResult);
    }
}

// Test: serialize<QMultiMap>() method -----------------------------------------------------------

using QMultiMapIntString = QMultiMap<int, QString>;
Q_DECLARE_METATYPE(QMultiMapIntString)

void TestSerialization::testSerializeQMultiMap()
{
    QFETCH(QMultiMapIntString, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeQMultiMap_data()
{
    QTest::addColumn<QMultiMapIntString>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    QTest::newRow("empty") << QMultiMapIntString() << QJsonValue(QJsonObject());

    {
        const QMultiMapIntString input
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
        const QJsonObject expectedResult
        {
            { "-1", QJsonArray { "a3", "a2", "a1" } },
            {  "0", QJsonArray { "b3", "b2", "b1" } },
            {  "1", QJsonArray { "c3", "c2", "c1" } }
        };
        QTest::newRow("non-empty") << input << QJsonValue(expectedResult);
    }
}

// Test: serialize<QMultiHash>() method -----------------------------------------------------------

using QMultiHashIntString = QMultiHash<int, QString>;
Q_DECLARE_METATYPE(QMultiHashIntString)

void TestSerialization::testSerializeQMultiHash()
{
    QFETCH(QMultiHashIntString, input);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::serialize(input);
    QCOMPARE(result, expectedResult);
}

void TestSerialization::testSerializeQMultiHash_data()
{
    QTest::addColumn<QMultiHashIntString>("input");
    QTest::addColumn<QJsonValue>("expectedResult");

    QTest::newRow("empty") << QMultiHashIntString() << QJsonValue(QJsonObject());

    {
        const QMultiHashIntString input
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
        const QJsonObject expectedResult
        {
            { "-1", QJsonArray { "a3", "a2", "a1" } },
            {  "0", QJsonArray { "b3", "b2", "b1" } },
            {  "1", QJsonArray { "c3", "c2", "c1" } }
        };
        QTest::newRow("non-empty") << input << QJsonValue(expectedResult);
    }
}

// Main function -----------------------------------------------------------------------------------

QTEST_MAIN(TestSerialization)
#include "testSerialization.moc"
