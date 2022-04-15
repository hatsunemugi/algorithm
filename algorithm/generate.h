#ifndef GENERATE_H
#define GENERATE_H
#include <QObject>
#include<QRunnable>
#include<QString>
#include<QVector>
#include <QTimer>
#include<QRegularExpression>
#include<QRegularExpressionMatch>
class Generate : public QObject,public QRunnable
{
    Q_OBJECT
public:
    explicit Generate(QObject *parent = nullptr);
    void receive(QString);
    void enable(){abled=true;ability();}
    void disable(){abled=false;ability();}
    void ability();
    void filter();
protected:
    void run() override;
signals:
    void send(QString,QVector<int>);
    void able(bool);
private:
    int n;
    int left;
    int right;
    bool abled;
    QTimer *t;
    QString i,o;
    QVector<double> x;
    QVector<int> a;
    QRegularExpression number;
    QRegularExpressionMatch match;
};

#endif // GENERATE_H
