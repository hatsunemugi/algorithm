#ifndef KNAPSACK_H
#define KNAPSACK_H

#include <QObject>
#include<QRunnable>
#include<QString>
#include<QVector>
#include<QRegularExpression>
#include<QRegularExpressionMatch>
class KnapSack : public QObject,public QRunnable
{
    Q_OBJECT
public:
    explicit KnapSack(QObject *parent = nullptr);
    void receive(QString i);
    void knapSack();
    void traceBack();
    void filter();
    void o();
protected:
    void run() override;
signals:
    void send(QVector<QString>,QVector<QVector<int>>);
private:
    QString i;
    int n,C;
    QVector<int> w,v,x;
    QVector<QString> p;
    QVector<QVector<int>> m;
    QRegularExpression number;
    QRegularExpressionMatch match;
};

#endif // KNAPSACK_H
