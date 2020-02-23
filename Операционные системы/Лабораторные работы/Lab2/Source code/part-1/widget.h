#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtConcurrent>

namespace Ui {
class Widget;
}

class Widget : public QWidget {
  Q_OBJECT

public:
  explicit Widget(QWidget *parent = nullptr);
  ~Widget();

private:
  Ui::Widget *ui;

  QFuture<void> t1;

  QFuture<void> t2;
};

#endif // WIDGET_H
