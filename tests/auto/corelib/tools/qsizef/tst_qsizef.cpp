// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QSizeF>
#ifdef QVARIANT_H
# error "This test requires qsize.h to not include qvariant.h"
#endif

// don't assume <type_traits>
template <typename T, typename U>
constexpr inline bool my_is_same_v = false;
template <typename T>
constexpr inline bool my_is_same_v<T, T> = true;

#define CHECK(cvref) \
    static_assert(my_is_same_v<decltype(get<0>(std::declval<QSizeF cvref >())), qreal cvref >); \
    static_assert(my_is_same_v<decltype(get<1>(std::declval<QSizeF cvref >())), qreal cvref >)

CHECK(&);
CHECK(const &);
CHECK(&&);
CHECK(const &&);

#undef CHECK

#include <QTest>
#include <QtTest/private/qcomparisontesthelper_p.h>
#include <qsize.h>

Q_DECLARE_METATYPE(QMarginsF)

static constexpr qreal qreal_min = std::numeric_limits<qreal>::min();

class tst_QSizeF : public QObject
{
    Q_OBJECT
private slots:
    void compareCompiles();
    void compare_data();
    void compare();

    void fuzzyCompare_data();
    void fuzzyCompare();

    void isNull_data();
    void isNull();

    void fuzzyIsNull_data();
    void fuzzyIsNull();

    void scale();

    void expandedTo();
    void expandedTo_data();

    void boundedTo_data();
    void boundedTo();

    void grownOrShrunkBy_data();
    void grownOrShrunkBy();

    void transpose_data();
    void transpose();

    void structuredBinding();
};

void tst_QSizeF::compareCompiles()
{
    QTestPrivate::testEqualityOperatorsCompile<QSizeF>();
    QTestPrivate::testEqualityOperatorsCompile<QSizeF, QSize>();
}

void tst_QSizeF::compare_data()
{
    QTest::addColumn<QSizeF>("lhs");
    QTest::addColumn<QSizeF>("rhs");
    QTest::addColumn<bool>("result");
    QTest::addColumn<bool>("mixedResult");

    auto row = [&](QSizeF lhs, QSizeF rhs, bool res, bool mixedRes) {
        QString str;
        QDebug dbg(&str);
        dbg.nospace() << "(" << lhs.width() << ", " << lhs.height() << ") vs "
                      << "(" << rhs.width() << ", " << rhs.height() << ")";
        QTest::addRow("%s", str.toLatin1().constData()) << lhs << rhs << res << mixedRes;
    };

    row(QSizeF(0.0, 0.0), QSizeF(0.0, 0.0), true, true);
    row(QSizeF(1.0, 2.0), QSizeF(1.0, 2.0), true, true);
    row(QSizeF(1.0, -1.0), QSizeF(-1.0, 1.0), false, false);
    row(QSizeF(0.1, 1.1), QSizeF(0.1, 1.1), true, false);
    row(QSizeF(qreal_min, 0.0), QSizeF(0.0, -qreal_min), true, true);
}

void tst_QSizeF::compare()
{
    QFETCH(QSizeF, lhs);
    QFETCH(QSizeF, rhs);
    QFETCH(bool, result);
    QFETCH(bool, mixedResult);

    QT_TEST_EQUALITY_OPS(lhs, rhs, result);

    const QSize rhsFixed = rhs.toSize();
    QT_TEST_EQUALITY_OPS(lhs, rhsFixed, mixedResult);
}

void tst_QSizeF::fuzzyCompare_data()
{
    compare_data();
}

void tst_QSizeF::fuzzyCompare()
{
    QFETCH(QSizeF, lhs);
    QFETCH(QSizeF, rhs);
    QFETCH(bool, result);

    QCOMPARE_EQ(qFuzzyCompare(lhs, rhs), result);
}

void tst_QSizeF::isNull_data()
{
    QTest::addColumn<qreal>("width");
    QTest::addColumn<qreal>("height");
    QTest::addColumn<bool>("isNull");

    QTest::newRow("0, 0") << qreal(0.0) << qreal(0.0) << true;
    QTest::newRow("-0, -0") << qreal(-0.0) << qreal(-0.0) << true;
    QTest::newRow("0, -0") << qreal(0) << qreal(-0.0) << true;
    QTest::newRow("-0, 0") << qreal(-0.0) << qreal(0) << true;
    QTest::newRow("-0.1, 0") << qreal(-0.1) << qreal(0) << false;
    QTest::newRow("0, -0.1") << qreal(0) << qreal(-0.1) << false;
    QTest::newRow("0.1, 0") << qreal(0.1) << qreal(0) << false;
    QTest::newRow("0, 0.1") << qreal(0) << qreal(0.1) << false;
    QTest::newRow("qreal_min, -qreal_min") << qreal_min << -qreal_min << false;
}

void tst_QSizeF::isNull()
{
    QFETCH(qreal, width);
    QFETCH(qreal, height);
    QFETCH(bool, isNull);

    QSizeF size(width, height);
    QCOMPARE(size.width(), width);
    QCOMPARE(size.height(), height);
    QCOMPARE(size.isNull(), isNull);
}

void tst_QSizeF::fuzzyIsNull_data()
{
    QTest::addColumn<qreal>("width");
    QTest::addColumn<qreal>("height");
    QTest::addColumn<bool>("fuzzyNull");

    QTest::newRow("0, 0") << qreal(0.0) << qreal(0.0) << true;
    QTest::newRow("-0, -0") << qreal(-0.0) << qreal(-0.0) << true;
    QTest::newRow("0, -0") << qreal(0) << qreal(-0.0) << true;
    QTest::newRow("-0, 0") << qreal(-0.0) << qreal(0) << true;
    QTest::newRow("-0.1, 0") << qreal(-0.1) << qreal(0) << false;
    QTest::newRow("0, -0.1") << qreal(0) << qreal(-0.1) << false;
    QTest::newRow("0.1, 0") << qreal(0.1) << qreal(0) << false;
    QTest::newRow("0, 0.1") << qreal(0) << qreal(0.1) << false;
    QTest::newRow("qreal_min, -qreal_min") << qreal_min << -qreal_min << true;
}

void tst_QSizeF::fuzzyIsNull()
{
    QFETCH(qreal, width);
    QFETCH(qreal, height);
    QFETCH(bool, fuzzyNull);

    QSizeF size(width, height);
    QCOMPARE(qFuzzyIsNull(size), fuzzyNull);
}

void tst_QSizeF::scale() {
    QSizeF t1(10.4, 12.8);
    t1.scale(60.6, 60.6, Qt::IgnoreAspectRatio);
    QCOMPARE(t1, QSizeF(60.6, 60.6));

    QSizeF t2(10.4, 12.8);
    t2.scale(43.52, 43.52, Qt::KeepAspectRatio);
    QCOMPARE(t2, QSizeF(35.36, 43.52));

    QSizeF t3(9.6, 12.48);
    t3.scale(31.68, 31.68, Qt::KeepAspectRatioByExpanding);
    QCOMPARE(t3, QSizeF(31.68, 41.184));

    QSizeF t4(12.8, 10.4);
    t4.scale(43.52, 43.52, Qt::KeepAspectRatio);
    QCOMPARE(t4, QSizeF(43.52, 35.36));

    QSizeF t5(12.48, 9.6);
    t5.scale(31.68, 31.68, Qt::KeepAspectRatioByExpanding);
    QCOMPARE(t5, QSizeF(41.184, 31.68));

    QSizeF t6(0.0, 0.0);
    t6.scale(200, 240, Qt::IgnoreAspectRatio);
    QCOMPARE(t6, QSizeF(200, 240));

    QSizeF t7(0.0, 0.0);
    t7.scale(200, 240, Qt::KeepAspectRatio);
    QCOMPARE(t7, QSizeF(200, 240));

    QSizeF t8(0.0, 0.0);
    t8.scale(200, 240, Qt::KeepAspectRatioByExpanding);
    QCOMPARE(t8, QSizeF(200, 240));
}


void tst_QSizeF::expandedTo_data() {
    QTest::addColumn<QSizeF>("input1");
    QTest::addColumn<QSizeF>("input2");
    QTest::addColumn<QSizeF>("expected");

    QTest::newRow("data0") << QSizeF(10.4, 12.8) << QSizeF(6.6, 4.4) << QSizeF(10.4, 12.8);
    QTest::newRow("data1") << QSizeF(0.0, 0.0) << QSizeF(6.6, 4.4) << QSizeF(6.6, 4.4);
    // This should pick the highest of w,h components independently of each other,
    // thus the result don't have to be equal to neither input1 nor input2.
    QTest::newRow("data3") << QSizeF(6.6, 4.4) << QSizeF(4.4, 6.6) << QSizeF(6.6, 6.6);
}

void tst_QSizeF::expandedTo() {
    QFETCH( QSizeF, input1);
    QFETCH( QSizeF, input2);
    QFETCH( QSizeF, expected);

    QCOMPARE( input1.expandedTo(input2), expected);
}

void tst_QSizeF::boundedTo_data() {
    QTest::addColumn<QSizeF>("input1");
    QTest::addColumn<QSizeF>("input2");
    QTest::addColumn<QSizeF>("expected");

    QTest::newRow("data0") << QSizeF(10.4, 12.8) << QSizeF(6.6, 4.4) << QSizeF(6.6, 4.4);
    QTest::newRow("data1") << QSizeF(0.0, 0.0) << QSizeF(6.6, 4.4) << QSizeF(0.0, 0.0);
    // This should pick the lowest of w,h components independently of each other,
    // thus the result don't have to be equal to neither input1 nor input2.
    QTest::newRow("data3") << QSizeF(6.6, 4.4) << QSizeF(4.4, 6.6) << QSizeF(4.4, 4.4);
}

void tst_QSizeF::boundedTo() {
    QFETCH( QSizeF, input1);
    QFETCH( QSizeF, input2);
    QFETCH( QSizeF, expected);

    QCOMPARE( input1.boundedTo(input2), expected);
}

void tst_QSizeF::grownOrShrunkBy_data()
{
    QTest::addColumn<QSizeF>("input");
    QTest::addColumn<QMarginsF>("margins");
    QTest::addColumn<QSizeF>("grown");
    QTest::addColumn<QSizeF>("shrunk");

    auto row = [](QSizeF i, QMarginsF m, QSizeF g, QSizeF s) {
        QTest::addRow("{%g,%g}/{%g,%g,%g,%g}", i.width(), i.height(),
                      m.left(), m.top(), m.right(), m.bottom())
                << i << m << g << s;
    };

    const QSizeF zero = {0, 0};
    const QSizeF some = {100, 200};
    const QMarginsF zeroMargins = {};
    const QMarginsF negative = {-1, -2, -3, -4};
    const QMarginsF positive = { 1,  2,  3,  4};

    row(zero, zeroMargins, zero, zero);
    row(zero, negative, {-4, -6}, { 4,  6});
    row(zero, positive, { 4,  6}, {-4, -6});
    row(some, zeroMargins, some, some);
    row(some, negative, { 96, 194}, {104, 206});
    row(some, positive, {104, 206}, { 96, 194});
}

void tst_QSizeF::grownOrShrunkBy()
{
    QFETCH(const QSizeF, input);
    QFETCH(const QMarginsF, margins);
    QFETCH(const QSizeF, grown);
    QFETCH(const QSizeF, shrunk);

    QCOMPARE(input.grownBy(margins), grown);
    QCOMPARE(input.shrunkBy(margins), shrunk);
    QCOMPARE(grown.shrunkBy(margins), input);
    QCOMPARE(shrunk.grownBy(margins), input);
}

void tst_QSizeF::transpose_data() {
    QTest::addColumn<QSizeF>("input1");
    QTest::addColumn<QSizeF>("expected");

    QTest::newRow("data0") << QSizeF(10.4, 12.8) << QSizeF(12.8, 10.4);
    QTest::newRow("data1") << QSizeF(0.0, 0.0) << QSizeF(0.0, 0.0);
    QTest::newRow("data3") << QSizeF(6.6, 4.4) << QSizeF(4.4, 6.6);
}

void tst_QSizeF::transpose() {
    QFETCH( QSizeF, input1);
    QFETCH( QSizeF, expected);

    // transpose() works only inplace and does not return anything, so we must do the operation itself before the compare.
    input1.transpose();
    QCOMPARE(input1 , expected);
}

void tst_QSizeF::structuredBinding()
{
    {
        QSizeF size(10.0, 20.0);
        auto [width, height] = size;
        QCOMPARE(width, 10.0);
        QCOMPARE(height, 20.0);
    }
    {
        QSizeF size(30.0, 40.0);
        auto &[width, height] = size;
        QCOMPARE(width, 30.0);
        QCOMPARE(height, 40.0);

        width = 100.0;
        height = 200.0;
        QCOMPARE(size.width(), 100.0);
        QCOMPARE(size.height(), 200.0);
    }
}

QTEST_APPLESS_MAIN(tst_QSizeF)
#include "tst_qsizef.moc"
