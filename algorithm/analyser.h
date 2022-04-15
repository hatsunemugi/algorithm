#ifndef ANALYSER_H
#define ANALYSER_H
#include <QObject>
#include<QRunnable>
#include<QString>
#include<QVector>
#include<QQueue>
#include<QRegularExpression>
#include<QRegularExpressionMatch>
class Lexical : public QObject,public QRunnable
{
    Q_OBJECT
public:
    explicit Lexical(QObject *parent = nullptr);
    void receive(QString);
    void filter();
    void analyse();
    int isLetter(QString);
    int isDigit(QString);
    int isOperator(QString);
    int isSeparator(QString);
    int isKeyword(QString);
    void o(int,QString);
    void organise();
protected:
    void run() override;
signals:
    void send(QVector<QVector<QString>>);
private:
    int state,cookie;
    QString line;
    QString filepath;
    QVector<QString> listt,listo;
    QQueue<QString> queue;
    QVector<QVector<QString>> w,vt;
    QVector<QString> Operators;
    QVector<QString> Separators;
    QVector<QString> Keywords;
    QRegularExpression re;
    QRegularExpressionMatch match;
};
class LL1 : public QObject,public QRunnable
{
    Q_OBJECT
public:
    explicit LL1(QObject *parent = nullptr);
    void receive(QString);
    void filter();
    void analyse();
protected:
    void run() override;
signals:
    void send(QVector<QVector<QString>>);
private:
    int state,cookie;
    QString line;
    QString filepath;
    QVector<QString> listt,listo;
    QQueue<QString> queue;
    QVector<QVector<QString>> w,vt;
    QRegularExpression re;
    QRegularExpressionMatch match;
};
#endif // ANALYSER_H
