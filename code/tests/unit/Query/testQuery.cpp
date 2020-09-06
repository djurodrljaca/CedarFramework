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
 * Contains unit tests for Query class
 */

// Cedar Framework includes
#include <CedarFramework/Query.hpp>

// Qt includes
#include <QtCore/QDebug>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>
#include <QtTest/QTest>

// System includes

// Forward declarations

// Macros

// Test class declaration --------------------------------------------------------------------------

class TestQuery : public QObject
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
    void testQueryHasNodeByIndex();
    void testQueryHasNodeByIndex_data();

    void testQueryHasNodeByName();
    void testQueryHasNodeByName_data();

    void testQueryHasNodeByPathVariantList();
    void testQueryHasNodeByPathVariantList_data();

    void testQueryHasNodeByPathStringList();
    void testQueryHasNodeByPathStringList_data();

    void testQueryGetNodeByIndex();
    void testQueryGetNodeByIndex_data();

    void testQueryGetNodeByName();
    void testQueryGetNodeByName_data();

    void testQueryGetNodeByPathVariantList();
    void testQueryGetNodeByPathVariantList_data();

    void testQueryGetNodeByPathStringList();
    void testQueryGetNodeByPathStringList_data();
};

// Test Case init/cleanup methods ------------------------------------------------------------------

void TestQuery::initTestCase()
{
}

void TestQuery::cleanupTestCase()
{
}

// Test init/cleanup methods -----------------------------------------------------------------------

void TestQuery::init()
{
}

void TestQuery::cleanup()
{
}

// Test: hasNode(input, index) method --------------------------------------------------------------

void TestQuery::testQueryHasNodeByIndex()
{
    QFETCH(QJsonValue, input);
    QFETCH(int, index);
    QFETCH(bool, expectedResult);

    const bool result = CedarFramework::hasNode(input, index);
    QCOMPARE(result, expectedResult);
}

void TestQuery::testQueryHasNodeByIndex_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<int>("index");
    QTest::addColumn<bool>("expectedResult");

    const QJsonValue input = QJsonArray { true, 0, "a" };

    // Positive tests
    QTest::newRow("0") << input << 0 << true;
    QTest::newRow("1") << input << 1 << true;
    QTest::newRow("2") << input << 2 << true;

    // Negative tests
    QTest::newRow("-1") << input << -1 << false;
    QTest::newRow("3") << input << 5 << false;
}

// Test: hasNode(input, name) method ---------------------------------------------------------------

void TestQuery::testQueryHasNodeByName()
{
    QFETCH(QJsonValue, input);
    QFETCH(QString, name);
    QFETCH(bool, expectedResult);

    const bool result = CedarFramework::hasNode(input, name);
    QCOMPARE(result, expectedResult);
}

void TestQuery::testQueryHasNodeByName_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QString>("name");
    QTest::addColumn<bool>("expectedResult");

    const QJsonValue input
    {
        QJsonObject
        {
            { "a", true },
            { "b", 0 },
            { "c", "a" }
        }
    };

    // Positive tests
    QTest::newRow("a") << input << "a" << true;
    QTest::newRow("b") << input << "b" << true;
    QTest::newRow("c") << input << "c" << true;

    // Negative tests
    QTest::newRow("A") << input << "A" << false;
    QTest::newRow("d") << input << "A" << false;
    QTest::newRow("0") << input << "0" << false;
}

// Test: hasNode(input, path) method ---------------------------------------------------------------

void TestQuery::testQueryHasNodeByPathVariantList()
{
    QFETCH(QJsonValue, input);
    QFETCH(QVariantList, path);
    QFETCH(bool, expectedResult);

    const bool result = CedarFramework::hasNode(input, path);
    QCOMPARE(result, expectedResult);
}

void TestQuery::testQueryHasNodeByPathVariantList_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QVariantList>("path");
    QTest::addColumn<bool>("expectedResult");

    const QJsonValue input
    {
        QJsonObject
        {
            { "a", true },
            {
                "b", QJsonArray
                {
                    1,
                    QJsonObject
                    {
                        { "x", QJsonArray { 1, 2, 3 } },
                        { "y", QJsonObject { { "a", 1 }, { "b", 2 } } },
                        { "z", "z" }
                    },
                    true
                }
            },
            { "c", "a" },
            {
                "d", QJsonObject
                {
                    { "x", QJsonArray { 1, 2, 3 } },
                    { "y", QJsonObject { { "a", 1 }, { "b", 2 }, { "7", "x"} } },
                    { "z", "z" }
                }
            }
        }
    };

    // Positive tests
    QTest::newRow("a") << input << QVariantList { "a" } << true;
    QTest::newRow("b") << input << QVariantList { "b" } << true;
    QTest::newRow("b/0") << input << QVariantList { "b", 0 } << true;
    QTest::newRow("b/1") << input << QVariantList { "b", 1 } << true;
    QTest::newRow("b/'2'") << input << QVariantList { "b", "2" } << true;
    QTest::newRow("b/1/x") << input << QVariantList { "b", 1, "x" } << true;
    QTest::newRow("b/1/x/1") << input << QVariantList { "b", 1, "x", 1 } << true;
    QTest::newRow("b/1/y") << input << QVariantList { "b", 1, "y" } << true;
    QTest::newRow("b/1/y/a") << input << QVariantList { "b", 1, "y", "a" } << true;
    QTest::newRow("c") << input << QVariantList { "c" } << true;
    QTest::newRow("d") << input << QVariantList { "d" } << true;
    QTest::newRow("d/x") << input << QVariantList { "d", "x" } << true;
    QTest::newRow("d/x/0") << input << QVariantList { "d", "x", 0 } << true;
    QTest::newRow("d/y") << input << QVariantList { "d", "y" } << true;
    QTest::newRow("d/y/b") << input << QVariantList { "d", "y", "b" } << true;
    QTest::newRow("d/y/7") << input << QVariantList { "d", "y", 7 } << true;
    QTest::newRow("d/z") << input << QVariantList { "d", "z" } << true;

    // Negative tests
    QTest::newRow("x") << input << QVariantList { "x" } << false;
    QTest::newRow("a/1") << input << QVariantList { "a", 1 } << false;
    QTest::newRow("a/a") << input << QVariantList { "a", "a" } << false;
    QTest::newRow("d/x/3") << input << QVariantList { "d", "x", 3 } << false;
    QTest::newRow("d/x/a") << input << QVariantList { "d", "x", "a" } << false;
    QTest::newRow("d/y/c") << input << QVariantList { "d", "y", "c" } << false;
    QTest::newRow("d/y/1") << input << QVariantList { "d", "y", 1 } << false;
    QTest::newRow("d/QBitArray") << input << QVariantList { "d", QBitArray() } << false;
}

// Test: hasNode(input, path) method ---------------------------------------------------------------

void TestQuery::testQueryHasNodeByPathStringList()
{
    QFETCH(QJsonValue, input);
    QFETCH(QStringList, path);
    QFETCH(bool, expectedResult);

    const bool result = CedarFramework::hasNode(input, path);
    QCOMPARE(result, expectedResult);
}

void TestQuery::testQueryHasNodeByPathStringList_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QStringList>("path");
    QTest::addColumn<bool>("expectedResult");

    const QJsonValue input
    {
        QJsonObject
        {
            { "a", true },
            {
                "b", QJsonArray
                {
                    1,
                    QJsonObject
                    {
                        { "x", QJsonArray { 1, 2, 3 } },
                        { "y", QJsonObject { { "a", 1 }, { "b", 2 } } },
                        { "z", "z" }
                    },
                    true
                }
            },
            { "c", "a" },
            {
                "d", QJsonObject
                {
                    { "x", QJsonArray { 1, 2, 3 } },
                    { "y", QJsonObject { { "a", 1 }, { "b", 2 }, { "7", "x"} } },
                    { "z", "z" }
                }
            }
        }
    };

    // Positive tests
    QTest::newRow("a") << input << QStringList { "a" } << true;
    QTest::newRow("b") << input << QStringList { "b" } << true;
    QTest::newRow("b/0") << input << QStringList { "b", "0" } << true;
    QTest::newRow("b/1") << input << QStringList { "b", "1" } << true;
    QTest::newRow("b/'2'") << input << QStringList { "b", "2" } << true;
    QTest::newRow("b/1/x") << input << QStringList { "b", "1", "x" } << true;
    QTest::newRow("b/1/x/1") << input << QStringList { "b", "1", "x", "1" } << true;
    QTest::newRow("b/1/y") << input << QStringList { "b", "1", "y" } << true;
    QTest::newRow("b/1/y/a") << input << QStringList { "b", "1", "y", "a" } << true;
    QTest::newRow("c") << input << QStringList { "c" } << true;
    QTest::newRow("d") << input << QStringList { "d" } << true;
    QTest::newRow("d/x") << input << QStringList { "d", "x" } << true;
    QTest::newRow("d/x/0") << input << QStringList { "d", "x", "0" } << true;
    QTest::newRow("d/y") << input << QStringList { "d", "y" } << true;
    QTest::newRow("d/y/b") << input << QStringList { "d", "y", "b" } << true;
    QTest::newRow("d/y/7") << input << QStringList { "d", "y", "7" } << true;
    QTest::newRow("d/z") << input << QStringList { "d", "z" } << true;

    // Negative tests
    QTest::newRow("x") << input << QStringList { "x" } << false;
    QTest::newRow("a/1") << input << QStringList { "a", "1" } << false;
    QTest::newRow("a/a") << input << QStringList { "a", "a" } << false;
    QTest::newRow("d/x/3") << input << QStringList { "d", "x", "3" } << false;
    QTest::newRow("d/x/a") << input << QStringList { "d", "x", "a" } << false;
    QTest::newRow("d/y/c") << input << QStringList { "d", "y", "c" } << false;
    QTest::newRow("d/y/1") << input << QStringList { "d", "y", "1" } << false;
    QTest::newRow("d/QBitArray") << input << QStringList { "d", "" } << false;
}

// Test: getNode(input, index) method --------------------------------------------------------------

void TestQuery::testQueryGetNodeByIndex()
{
    QFETCH(QJsonValue, input);
    QFETCH(int, index);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::getNode(input, index);
    QCOMPARE(result, expectedResult);
}

void TestQuery::testQueryGetNodeByIndex_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<int>("index");
    QTest::addColumn<QJsonValue>("expectedResult");

    const QJsonValue input = QJsonArray { true, 0, "a" };

    // Positive tests
    QTest::newRow("0") << input << 0 << QJsonValue(true);
    QTest::newRow("1") << input << 1 << QJsonValue(0);
    QTest::newRow("2") << input << 2 << QJsonValue("a");

    // Negative tests
    QTest::newRow("-1") << input << -1 << QJsonValue(QJsonValue::Undefined);
    QTest::newRow("3") << input << 5 << QJsonValue(QJsonValue::Undefined);
}

// Test: getNode(input, name) method ---------------------------------------------------------------

void TestQuery::testQueryGetNodeByName()
{
    QFETCH(QJsonValue, input);
    QFETCH(QString, name);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::getNode(input, name);
    QCOMPARE(result, expectedResult);
}

void TestQuery::testQueryGetNodeByName_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QString>("name");
    QTest::addColumn<QJsonValue>("expectedResult");

    const QJsonValue input
    {
        QJsonObject
        {
            { "a", true },
            { "b", 0 },
            { "c", "a" }
        }
    };

    // Positive tests
    QTest::newRow("a") << input << "a" << QJsonValue(true);
    QTest::newRow("b") << input << "b" << QJsonValue(0);
    QTest::newRow("c") << input << "c" << QJsonValue("a");

    // Negative tests
    QTest::newRow("A") << input << "A" << QJsonValue(QJsonValue::Undefined);
    QTest::newRow("d") << input << "A" << QJsonValue(QJsonValue::Undefined);
    QTest::newRow("0") << input << "0" << QJsonValue(QJsonValue::Undefined);
}

// Test: getNode(input, path) method ---------------------------------------------------------------

void TestQuery::testQueryGetNodeByPathVariantList()
{
    QFETCH(QJsonValue, input);
    QFETCH(QVariantList, path);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::getNode(input, path);
    QCOMPARE(result, expectedResult);
}

void TestQuery::testQueryGetNodeByPathVariantList_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QVariantList>("path");
    QTest::addColumn<QJsonValue>("expectedResult");

    const QJsonValue input
    {
        QJsonObject
        {
            { "a", true },
            {
                "b", QJsonArray
                {
                    1,
                    QJsonObject
                    {
                        { "x", QJsonArray { 1, 2, 3 } },
                        { "y", QJsonObject { { "a", 1 }, { "b", 2 } } },
                        { "z", "z" }
                    },
                    true
                }
            },
            { "c", "a" },
            {
                "d", QJsonObject
                {
                    { "x", QJsonArray { 1, 2, 3 } },
                    { "y", QJsonObject { { "a", 1 }, { "b", 2 }, { "7", "x"} } },
                    { "z", "z" }
                }
            }
        }
    };

    // Positive tests
    QTest::newRow("a") << input << QVariantList { "a" } << QJsonValue(true);

    {
        const QJsonArray expectedResult
        {
            1,
            QJsonObject
            {
                { "x", QJsonArray { 1, 2, 3 } },
                { "y", QJsonObject { { "a", 1 }, { "b", 2 } } },
                { "z", "z" }
            },
            true
        };
        QTest::newRow("b") << input << QVariantList { "b" } << QJsonValue(expectedResult);
    }

    QTest::newRow("b/0") << input << QVariantList { "b", 0 } << QJsonValue(1);

    {
        const QJsonObject expectedResult
        {
            { "x", QJsonArray { 1, 2, 3 } },
            { "y", QJsonObject { { "a", 1 }, { "b", 2 } } },
            { "z", "z" }
        };
        QTest::newRow("b/1") << input << QVariantList { "b", 1 } << QJsonValue(expectedResult);
    }

    QTest::newRow("b/'2'") << input << QVariantList { "b", "2" } << QJsonValue(true);
    QTest::newRow("b/1/x") << input << QVariantList { "b", 1, "x" }
                           << QJsonValue(QJsonArray { 1, 2, 3 });
    QTest::newRow("b/1/x/1") << input << QVariantList { "b", 1, "x", 1 } << QJsonValue(2);
    QTest::newRow("b/1/y") << input << QVariantList { "b", 1, "y" }
                           << QJsonValue(QJsonObject { { "a", 1 }, { "b", 2 } });
    QTest::newRow("b/1/y/a") << input << QVariantList { "b", 1, "y", "a" } << QJsonValue(1);
    QTest::newRow("c") << input << QVariantList { "c" } << QJsonValue("a");

    {
        const QJsonObject expectedResult
        {
            { "x", QJsonArray { 1, 2, 3 } },
            { "y", QJsonObject { { "a", 1 }, { "b", 2 }, { "7", "x"} } },
            { "z", "z" }
        };
        QTest::newRow("d") << input << QVariantList { "d" } << QJsonValue(expectedResult);
    }

    QTest::newRow("d/x") << input << QVariantList { "d", "x" }
                         << QJsonValue(QJsonArray { 1, 2, 3 });
    QTest::newRow("d/x/0") << input << QVariantList { "d", "x", 0 } << QJsonValue(1);
    QTest::newRow("d/y") << input << QVariantList { "d", "y" }
                         << QJsonValue(QJsonObject { { "a", 1 }, { "b", 2 }, { "7", "x"} });
    QTest::newRow("d/y/b") << input << QVariantList { "d", "y", "b" } << QJsonValue(2);
    QTest::newRow("d/y/7") << input << QVariantList { "d", "y", 7 } << QJsonValue("x");
    QTest::newRow("d/z") << input << QVariantList { "d", "z" } << QJsonValue("z");

    // Negative tests
    QTest::newRow("x") << input << QVariantList { "x" } << QJsonValue(QJsonValue::Undefined);
    QTest::newRow("a/1") << input << QVariantList { "a", 1 } << QJsonValue(QJsonValue::Undefined);
    QTest::newRow("a/a") << input << QVariantList { "a", "a" } << QJsonValue(QJsonValue::Undefined);
    QTest::newRow("d/x/3") << input << QVariantList { "d", "x", 3 }
                           << QJsonValue(QJsonValue::Undefined);
    QTest::newRow("d/x/a") << input << QVariantList { "d", "x", "a" }
                           << QJsonValue(QJsonValue::Undefined);
    QTest::newRow("d/y/c") << input << QVariantList { "d", "y", "c" }
                           << QJsonValue(QJsonValue::Undefined);
    QTest::newRow("d/y/1") << input << QVariantList { "d", "y", 1 }
                           << QJsonValue(QJsonValue::Undefined);
    QTest::newRow("d/QBitArray") << input << QVariantList { "d", QBitArray() }
                                 << QJsonValue(QJsonValue::Undefined);
}

// Test: getNode(input, path) method ---------------------------------------------------------------

void TestQuery::testQueryGetNodeByPathStringList()
{
    QFETCH(QJsonValue, input);
    QFETCH(QStringList, path);
    QFETCH(QJsonValue, expectedResult);

    const auto result = CedarFramework::getNode(input, path);
    QCOMPARE(result, expectedResult);
}

void TestQuery::testQueryGetNodeByPathStringList_data()
{
    QTest::addColumn<QJsonValue>("input");
    QTest::addColumn<QStringList>("path");
    QTest::addColumn<QJsonValue>("expectedResult");

    const QJsonValue input
    {
        QJsonObject
        {
            { "a", true },
            {
                "b", QJsonArray
                {
                    1,
                    QJsonObject
                    {
                        { "x", QJsonArray { 1, 2, 3 } },
                        { "y", QJsonObject { { "a", 1 }, { "b", 2 } } },
                        { "z", "z" }
                    },
                    true
                }
            },
            { "c", "a" },
            {
                "d", QJsonObject
                {
                    { "x", QJsonArray { 1, 2, 3 } },
                    { "y", QJsonObject { { "a", 1 }, { "b", 2 }, { "7", "x"} } },
                    { "z", "z" }
                }
            }
        }
    };

    // Positive tests
    QTest::newRow("a") << input << QStringList { "a" } << QJsonValue(true);

    {
        const QJsonArray expectedResult
        {
            1,
            QJsonObject
            {
                { "x", QJsonArray { 1, 2, 3 } },
                { "y", QJsonObject { { "a", 1 }, { "b", 2 } } },
                { "z", "z" }
            },
            true
        };
        QTest::newRow("b") << input << QStringList { "b" } << QJsonValue(expectedResult);
    }

    QTest::newRow("b/0") << input << QStringList { "b", "0" } << QJsonValue(1);

    {
        const QJsonObject expectedResult
        {
            { "x", QJsonArray { 1, 2, 3 } },
            { "y", QJsonObject { { "a", 1 }, { "b", 2 } } },
            { "z", "z" }
        };
        QTest::newRow("b/1") << input << QStringList { "b", "1" } << QJsonValue(expectedResult);
    }

    QTest::newRow("b/'2'") << input << QStringList { "b", "2" } << QJsonValue(true);
    QTest::newRow("b/1/x") << input << QStringList { "b", "1", "x" }
                           << QJsonValue(QJsonArray { 1, 2, 3 });
    QTest::newRow("b/1/x/1") << input << QStringList { "b", "1", "x", "1" } << QJsonValue(2);
    QTest::newRow("b/1/y") << input << QStringList { "b", "1", "y" }
                           << QJsonValue(QJsonObject { { "a", 1 }, { "b", 2 } });
    QTest::newRow("b/1/y/a") << input << QStringList { "b", "1", "y", "a" } << QJsonValue(1);
    QTest::newRow("c") << input << QStringList { "c" } << QJsonValue("a");

    {
        const QJsonObject expectedResult
        {
            { "x", QJsonArray { 1, 2, 3 } },
            { "y", QJsonObject { { "a", 1 }, { "b", 2 }, { "7", "x"} } },
            { "z", "z" }
        };
        QTest::newRow("d") << input << QStringList { "d" } << QJsonValue(expectedResult);
    }

    QTest::newRow("d/x") << input << QStringList { "d", "x" }
                         << QJsonValue(QJsonArray { 1, 2, 3 });
    QTest::newRow("d/x/0") << input << QStringList { "d", "x", "0" } << QJsonValue(1);
    QTest::newRow("d/y") << input << QStringList { "d", "y" }
                         << QJsonValue(QJsonObject { { "a", 1 }, { "b", 2 }, { "7", "x"} });
    QTest::newRow("d/y/b") << input << QStringList { "d", "y", "b" } << QJsonValue(2);
    QTest::newRow("d/y/7") << input << QStringList { "d", "y", "7" } << QJsonValue("x");
    QTest::newRow("d/z") << input << QStringList { "d", "z" } << QJsonValue("z");

    // Negative tests
    QTest::newRow("x") << input << QStringList { "x" } << QJsonValue(QJsonValue::Undefined);
    QTest::newRow("a/1") << input << QStringList { "a", "1" } << QJsonValue(QJsonValue::Undefined);
    QTest::newRow("a/a") << input << QStringList { "a", "a" } << QJsonValue(QJsonValue::Undefined);
    QTest::newRow("d/x/3") << input << QStringList { "d", "x", "3" }
                           << QJsonValue(QJsonValue::Undefined);
    QTest::newRow("d/x/a") << input << QStringList { "d", "x", "a" }
                           << QJsonValue(QJsonValue::Undefined);
    QTest::newRow("d/y/c") << input << QStringList { "d", "y", "c" }
                           << QJsonValue(QJsonValue::Undefined);
    QTest::newRow("d/y/1") << input << QStringList { "d", "y", "1" }
                           << QJsonValue(QJsonValue::Undefined);
    QTest::newRow("d/QBitArray") << input << QStringList { "d", "" }
                                 << QJsonValue(QJsonValue::Undefined);
}

// Main function -----------------------------------------------------------------------------------

QTEST_MAIN(TestQuery)
#include "testQuery.moc"
