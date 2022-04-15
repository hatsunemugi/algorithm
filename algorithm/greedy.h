#ifndef GREEDY_H
#define GREEDY_H

#include <QObject>
#include<QRunnable>
#include<QString>
#include<QVector>
#include<QRegularExpression>
#include<QRegularExpressionMatch>
namespace greedy
{
class KnapSack : public QObject,public QRunnable
{
    Q_OBJECT
public:
    explicit KnapSack(QObject *parent = nullptr);
    void receive(QString i);
    void knapSack();
    void filter();
    void generateIndex();
    void o();
protected:
    void run() override;
signals:
    void send(QVector<QString>,QVector<QVector<double>>);
private:
    QString i;
    int n,C,cookie;
    QVector<double> x,index;
    QVector<QString> p;
    QVector<QVector<double>> vw;
    QRegularExpression number;
    QRegularExpressionMatch match;
};
class Selector : public QObject,public QRunnable
{
    Q_OBJECT
public:
    explicit Selector(QObject *parent = nullptr);
    void receive(QString i);
    void selector();
    void filter();
    void generateIndex();
    void o();
protected:
    void run() override;
signals:
    void send(QVector<QString>,QVector<QVector<double>>);
private:
    QString i;
    int n;
    QVector<double> x,index;
    QVector<QString> p;
    QVector<QVector<double>> sf;
    QRegularExpression number;
    QRegularExpressionMatch match;
};
}
#endif // GREEDY_H
