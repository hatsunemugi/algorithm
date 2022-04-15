#ifndef MATRIXCHAIN_H
#define MATRIXCHAIN_H
#include <QObject>
#include<QRunnable>
#include<QString>
#include<QVector>
#include<QRegularExpression>
#include<QRegularExpressionMatch>
class MatrixChain : public QObject,public QRunnable
{
    Q_OBJECT
public:
    explicit MatrixChain(QObject *parent = nullptr);
    void receive(QString i);
    void filter();
    void matrixChain();
    void Traceback (int i, int j);
    void translate();
protected:
    void run() override;
signals:
    void send(QString,QVector<QVector<int>>,QVector<QVector<int>>);
private:
    QString i;
    QVector<int> p;
    QVector<QVector<int>> m,s;
    QVector<QVector<QString>> q;
};

#endif // MATRIXCHAIN_H
