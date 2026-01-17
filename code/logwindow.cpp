#include "logwindow.h"
#include "mainwindow.h"
#include <QScreen>
#include <QGuiApplication>
#include <QVBoxLayout>

LogWindow::LogWindow(QWidget *parent)
    : QWidget(parent)
{
    //窗口透明 + 置顶（保留原有属性）
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    //初始化日志控件（保留原有逻辑）
    m_logTextEdit = new QPlainTextEdit(this);
    m_logTextEdit->setReadOnly(true);
    m_logTextEdit->setStyleSheet(R"(
        QPlainTextEdit {
            background-color: transparent;
            color: #ffffff;
            font-family: "Consolas";
            font-size: 14px;
            padding: 8px;
            border: none;
        }
    )");

    //布局（保留原有逻辑）
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_logTextEdit);
    layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(layout);

    // 固定窗口大小 + 左上方定位
    this->resize(1200*Ui::size, 400*Ui::size); // 窗口大小按需调整

    // 获取屏幕可用区域（排除任务栏）
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenRect = screen->availableGeometry();

    // 方式1：完全贴左上方（坐标 (0,0)）
    // this->move(screenRect.topLeft());

    // 方式2：左上方留边距（推荐，比如左/上各 20px，避免完全贴边）
    int marginX = 5*Ui::size; // 水平边距
    int marginY = 5*Ui::size; // 垂直边距
    this->move(screenRect.left() + marginX, screenRect.top() + marginY);
}

LogWindow::~LogWindow()
{
}

// 追加日志逻辑
void LogWindow::appendLog(const QString &logText, const QColor &textColor)
{
    QTextCursor cursor = m_logTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);

    QTextCharFormat format;
    format.setForeground(textColor);
    cursor.setCharFormat(format);

    cursor.insertText(logText + "\n");
    m_logTextEdit->setTextCursor(cursor);
    m_logTextEdit->ensureCursorVisible();
}
