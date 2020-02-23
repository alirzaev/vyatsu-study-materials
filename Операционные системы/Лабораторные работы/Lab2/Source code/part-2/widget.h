#ifndef WIDGET_H
#define WIDGET_H

#include <cstdint>
#include <map>

#include <QPixmap>
#include <QString>
#include <QTimer>
#include <QWidget>
#include <QtConcurrent>

#define SCENE_HEIGHT 300
#define SCENE_WIDTH 500
#define TEXT_HEIGHT 25
#define TEXT_WIDTH 85
#define PIXMAP_SIZE 32
#define HAT_SIZE 50
#define OFFSET 10
#define MAX_TIMESLICE 4000
#define SMOKING_TIMESLICE 800
#define EMITTER_TIMESLICE 600
#define WAITING_TIMESLICE 247
#define RENDER_TIMEOUT 50

#define SMOKING 1
#define WAITING 0
#define NONE 2

namespace Ui {
class Widget;
}

enum class SmokeItemTypes : size_t {
  Match = 0,
  Paper = 1,
  Tobacco = 2,
  None = 3
};

class Widget : public QWidget {
  Q_OBJECT

public:
  explicit Widget(QWidget *parent = nullptr);

  ~Widget();

signals:
  void stateChanged(int, int);

  void workFinished();

private:
  Ui::Widget *ui;

  QThreadPool pool;

  QTimer timer;

  QMetaObject::Connection connection;

  std::map<SmokeItemTypes, QPixmap> pixmaps = {
      {SmokeItemTypes::Match,
       QPixmap(":/rc/pictures/match.png").scaled(PIXMAP_SIZE, PIXMAP_SIZE)},
      {SmokeItemTypes::Paper,
       QPixmap(":/rc/pictures/paper.png").scaled(PIXMAP_SIZE, PIXMAP_SIZE)},
      {SmokeItemTypes::Tobacco,
       QPixmap(":/rc/pictures/tobacco.png").scaled(PIXMAP_SIZE, PIXMAP_SIZE)}};

  std::map<uint8_t, QString> states = {
      {WAITING, "Жду"}, {SMOKING, "Курю"}, {NONE, "Не хочу"}};

  QPixmap hatPixmap =
      QPixmap(":/rc/pictures/top-hat.png").scaled(HAT_SIZE, HAT_SIZE);

  void setupScene();

  void setupSignals();

  void startWork();

  void onWorkFinished();

  void startSmoker(int smoker);

  void updateState();
};

#endif // WIDGET_H
