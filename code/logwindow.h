#ifndef LOGWINDOW_H
#define LOGWINDOW_H

#include <QWidget>       // 替换 QMainWindow 为 QWidget
#include <QPlainTextEdit>
#include <QColor>

class LogWindow : public QWidget  // 继承 QWidget
{
    Q_OBJECT

public:
    LogWindow(QWidget *parent = nullptr);
    ~LogWindow();
    // 追加日志的核心接口
    void appendLog(const QString &logText, const QColor &textColor = Qt::white);

private:
    QPlainTextEdit *m_logTextEdit; // 日志显示控件
};

#endif // LOGWINDOW_H
