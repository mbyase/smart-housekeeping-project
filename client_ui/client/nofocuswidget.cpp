#include "nofocuswidget.h"

NoFocusWidget::NoFocusWidget(QWidget *parent)
    : BaseWindow(parent)
{
    ui.setupUi(this);
    // 初始化标题栏;
    initTitleBar();
}

void NoFocusWidget::initTitleBar()
{
    // 设置标题栏跑马灯效果，可以不设置;
    m_titleBar->setTitleRoll();

    m_titleBar->setTitleIcon(":/Resources/titleicon.png");
    m_titleBar->setTitleContent(QStringLiteral("前行中的小猪-前行之路还需前行"));
    m_titleBar->setButtonType(MIN_MAX_BUTTON);
    m_titleBar->setTitleWidth(this->width());
}
