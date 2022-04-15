#ifndef MAXSUM_H
#define MAXSUM_H

#include <QObject>
#include<QRunnable>
#include<QString>
#include<QVector>
#include<QRegularExpression>
#include<QRegularExpressionMatch>
class MaxSum : public QObject,public QRunnable
{
    Q_OBJECT
public:
    explicit MaxSum(QObject *parent = nullptr);
    void receive(QString i);
    void maxSum();
    void filter();
    void o();
protected:
    void run() override;
signals:
    void send(QVector<QString>,QVector<int>,QVector<QVector<QString>>);
private:
    int n,sum,besti,bestj;
    QString i;
    QVector<int> a;
    QVector<QString> q;
    QVector<QVector<QString>> p;
    QRegularExpression number;
};

#endif // MAXSUM_H
