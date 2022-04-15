#ifndef BACKTRACKING_H
#define BACKTRACKING_H

#include <QObject>
#include<QRunnable>
#include<QString>
#include<QVector>
#include<QQueue>
#include<QRegularExpression>
#include<QRegularExpressionMatch>
namespace backtracking
{
class Queen : public QObject,public QRunnable
{
    Q_OBJECT
public:
    explicit Queen(QObject *parent = nullptr);
    void receive(QString i);
    void backtrack(int k);
    bool place(int k);
    void filter();
    void o();
protected:
    void run() override;
signals:
    void send(QQueue<QVector<int>>);
    void update(int);
private:
    QString i;
    int n,sum;
    QVector<int> x;
    QQueue<QVector<int>> output;
    QRegularExpression number;
    QRegularExpressionMatch match;
};
class SubGroup: public QObject,public QRunnable
{
    Q_OBJECT
public:
    explicit SubGroup(QObject *parent = nullptr);
    void receive(QString i);
    void backtrack(int t);
    int rest(int);
    void filter();
    void o();
protected:
    void run() override;
signals:
    void send(QQueue<QVector<int>>);
    void update(int);
private:
    bool found;
    QString i;
    int n,c,currc;//当前累加值;
    QVector<int> x,w;
    QQueue<QVector<int>> output;
    QRegularExpression number;
    QRegularExpressionMatch match;
};
}
#endif // BACKTRACKING_H
