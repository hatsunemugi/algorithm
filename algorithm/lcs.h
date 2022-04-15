#ifndef LCS_H
#define LCS_H

#include <QObject>
#include<QRunnable>
#include<QString>
#include<QVector>
#include<QRegularExpression>
#include<QRegularExpressionMatch>
class lcs : public QObject,public QRunnable
{
    Q_OBJECT
public:
    explicit lcs(QObject *parent = nullptr);
    void LCSLength();
    void LCS(int i, int j);
    void LCS();
    void receive(QString i);
    void partition();
    void filter();
    void o();
    void filter(QString &p);
protected:
    void run() override;
signals:
    void send(QString o,QVector<QVector<QString>> p);
private:
    QString i;
    QString x;
    QString y;
    QString output;
    QRegularExpression re;
    QVector<QVector<int>> c;
    QVector<QVector<QString>> b,p;
};

#endif // LCS_H
