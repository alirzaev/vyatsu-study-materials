#include <algorithm>
#include <atomic>
#include <mutex>
#include <vector>

#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QPixmap>
#include <QPoint>
#include <QString>
#include <QThread>
#include <QTimer>
#include <QtConcurrent>

#include "ui_widget.h"
#include "widget.h"

struct SmokeItem {
  QPointF pos;
  SmokeItemTypes type;
};

struct SmokerInfo {
  std::pair<SmokeItem, SmokeItem> items;
  SmokeItem missed;
  QPoint hatPos;
  size_t num;
};

static const QPointF SCENE_CENTER{SCENE_WIDTH / 2 - PIXMAP_SIZE / 2,
                                  SCENE_HEIGHT / 2 - PIXMAP_SIZE / 2};

static std::mutex mutex;

static bool needsToStop;

static std::atomic_uint stopped{0xF};

static unsigned int timeslices[3];

static SmokeItemTypes currentSmokeItem;

static uint8_t isSmoking[3] = {WAITING, WAITING, WAITING};

static QGraphicsItem *tablePixmapItem = nullptr;

static QGraphicsItem *smokersPixmapItems[3];

static QGraphicsItem *statePixmapItems[3];

static int to_int(SmokeItemTypes item) { return static_cast<int>(item); }

static unsigned int computeTimeslice(int value) {
  return static_cast<unsigned int>(300 + MAX_TIMESLICE * (100 - value) / 100);
}

static std::vector<SmokerInfo> smokers = {
    {{SmokeItem{{OFFSET, OFFSET + HAT_SIZE}, SmokeItemTypes::Match},
      SmokeItem{{OFFSET + PIXMAP_SIZE, OFFSET + HAT_SIZE},
                SmokeItemTypes::Paper}},
     SmokeItem{{OFFSET + 2 * PIXMAP_SIZE, OFFSET + HAT_SIZE},
               SmokeItemTypes::Tobacco},
     QPoint{OFFSET, OFFSET},
     0},
    {{SmokeItem{{OFFSET + HAT_SIZE, SCENE_HEIGHT - (OFFSET + 2 * PIXMAP_SIZE)},
                SmokeItemTypes::Paper},
      SmokeItem{{OFFSET + HAT_SIZE, SCENE_HEIGHT - (OFFSET + PIXMAP_SIZE)},
                SmokeItemTypes::Tobacco}},
     SmokeItem{{OFFSET + HAT_SIZE, SCENE_HEIGHT - (OFFSET + 3 * PIXMAP_SIZE)},
               SmokeItemTypes::Match},
     QPoint{OFFSET, SCENE_HEIGHT - (OFFSET + 2 * PIXMAP_SIZE)},
     1},
    {{SmokeItem{{SCENE_WIDTH - (OFFSET + 2 * PIXMAP_SIZE),
                 SCENE_HEIGHT / 2 + HAT_SIZE / 2},
                SmokeItemTypes::Match},
      SmokeItem{{SCENE_WIDTH - (OFFSET + PIXMAP_SIZE),
                 SCENE_HEIGHT / 2 + HAT_SIZE / 2},
                SmokeItemTypes::Tobacco}},
     SmokeItem{{SCENE_WIDTH - (OFFSET + 3 * PIXMAP_SIZE),
                SCENE_HEIGHT / 2 + HAT_SIZE / 2},
               SmokeItemTypes::Paper},
     QPoint{SCENE_WIDTH - (OFFSET + 2 * PIXMAP_SIZE),
            SCENE_HEIGHT / 2 - HAT_SIZE / 2},
     2}};

Widget::Widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::Widget), pool(this) {
  ui->setupUi(this);

  setupSignals();
  setupScene();

  pool.setMaxThreadCount(4);
  timer.setInterval(RENDER_TIMEOUT);
}

Widget::~Widget() {
  needsToStop = true;
  timer.stop();
  while (stopped != 0xF) {
  }
  delete ui;
}

void Widget::setupScene() {
  auto *view = ui->graphicsView;

  auto *scene = new QGraphicsScene();

  view->setScene(scene);

  scene->addRect(0, -20, SCENE_WIDTH, SCENE_HEIGHT + 20);
  scene->addEllipse(180, 80, 140, 140);

  for (const auto &smoker : smokers) {
    // set hat
    auto *hat = scene->addPixmap(hatPixmap);
    hat->setPos(smoker.hatPos);
    // set first item
    auto first = smoker.items.first;
    auto *firstItem = scene->addPixmap(pixmaps[first.type]);
    firstItem->setPos(first.pos);
    // set second item
    auto second = smoker.items.second;
    auto *secondItem = scene->addPixmap(pixmaps[second.type]);
    secondItem->setPos(second.pos);
  }
}

void Widget::setupSignals() {
  connect(this, &Widget::stateChanged, this, &Widget::updateState,
          Qt::QueuedConnection);
  connect(ui->startButton, &QPushButton::clicked, this, &Widget::startWork);
  connect(this, &Widget::workFinished, this, &Widget::onWorkFinished);
  connect(&timer, &QTimer::timeout, this, &Widget::updateState);
  connect(ui->stopButton, &QPushButton::clicked, this, [this]() {
    needsToStop = true;
    ui->stopButton->setDisabled(true);
    this->timer.stop();

    QtConcurrent::run([this]() {
      while (stopped != 0xF) {
      }
      emit this->workFinished();
    });
  });
  connect(ui->speedSlider, &QSlider::valueChanged, this, [](int value) {
    timeslices[0] = computeTimeslice(value);
    qDebug() << "timeslice[0]: " << timeslices[0];
  });
  connect(ui->speedSlider2, &QSlider::valueChanged, this, [](int value) {
    timeslices[1] = computeTimeslice(value);
    qDebug() << "timeslice[1]: " << timeslices[1];
  });
  connect(ui->speedSlider3, &QSlider::valueChanged, this, [](int value) {
    timeslices[2] = computeTimeslice(value);
    qDebug() << "timeslice[2]: " << timeslices[2];
  });
  ui->speedSlider->setValue(50);
  ui->speedSlider2->setValue(50);
  ui->speedSlider3->setValue(50);
}

void Widget::onWorkFinished() {
  ui->stopButton->setDisabled(true);
  ui->startButton->setEnabled(true);

  if (tablePixmapItem &&
      tablePixmapItem->scene() == ui->graphicsView->scene()) {
    ui->graphicsView->scene()->removeItem(tablePixmapItem);
    tablePixmapItem = nullptr;
  }
}

void Widget::startWork() {
  timeslices[0] = computeTimeslice(ui->speedSlider->value());
  timeslices[1] = computeTimeslice(ui->speedSlider2->value());
  timeslices[2] = computeTimeslice(ui->speedSlider3->value());
  qDebug() << timeslices[0] << timeslices[1] << timeslices[2];

  ui->stopButton->setEnabled(true);
  ui->startButton->setDisabled(true);

  needsToStop = false;
  stopped = 0x0;
  currentSmokeItem = SmokeItemTypes::None;

  QtConcurrent::run(&this->pool, []() {
    std::vector<SmokeItemTypes> items = {
        SmokeItemTypes::Tobacco, SmokeItemTypes::Match, SmokeItemTypes::Paper};
    QString names[] = {"Tobacco", "Match", "Paper"};

    while (!needsToStop) {
      mutex.lock();
      currentSmokeItem = items[rand() % 3];
      qDebug() << "emitted item" << names[to_int(currentSmokeItem)];
      mutex.unlock();

      QThread::msleep(EMITTER_TIMESLICE);
    }

    stopped |= 1 << 3;
    qDebug() << "emitter finished";
  });

  for (int i = 0; i < 3; ++i) {
    QtConcurrent::run(&this->pool, [this, i]() { this->startSmoker(i); });
  }

  timer.start();
}

void Widget::startSmoker(int smoker) {
  auto index = static_cast<unsigned int>(smoker);
  auto missedItem = smokers[index].missed.type;

  while (!needsToStop) {
    mutex.lock();

    isSmoking[index] = WAITING;

    mutex.unlock();

    QThread::msleep(WAITING_TIMESLICE);

    mutex.lock();

    if (currentSmokeItem == missedItem) {
      isSmoking[index] = SMOKING;
      currentSmokeItem = SmokeItemTypes::None;
      qDebug() << "smoker" << index << "took item";

      mutex.unlock();

      QThread::msleep(SMOKING_TIMESLICE);

      mutex.lock();

      isSmoking[index] = NONE;

      mutex.unlock();

      QThread::msleep(timeslices[index]);
    } else {
      mutex.unlock();
    }
  }

  stopped |= 1u << index;
  qDebug() << "smoker" << index << "finished";
}

void Widget::updateState() {
  if (needsToStop) {
    return;
  }

  mutex.lock();

  auto item = currentSmokeItem;
  uint8_t localIsSmoking[3] = {isSmoking[0], isSmoking[1], isSmoking[2]};

  mutex.unlock();

  auto *scene = ui->graphicsView->scene();

  // Update item on the table
  if (tablePixmapItem && tablePixmapItem->scene() == scene) {
    scene->removeItem(tablePixmapItem);
  }
  tablePixmapItem = new QGraphicsPixmapItem(pixmaps[item]);
  scene->addItem(tablePixmapItem);
  tablePixmapItem->setPos(SCENE_CENTER);

  // Update smokers' items
  for (size_t i = 0; i < 3; ++i) {
    if (smokersPixmapItems[i] && smokersPixmapItems[i]->scene() == scene) {
      scene->removeItem(smokersPixmapItems[i]);
      delete smokersPixmapItems[i];
      smokersPixmapItems[i] = nullptr;
    }
  }
  // Update state's items
  for (size_t i = 0; i < 3; ++i) {
    auto smokerInfo = smokers[i];
    if (statePixmapItems[i] && statePixmapItems[i]->scene() == scene) {
      scene->removeItem(statePixmapItems[i]);
      delete statePixmapItems[i];
    }
    statePixmapItems[i] = new QGraphicsTextItem(
        QString("%1: %2").arg(i + 1).arg(states[localIsSmoking[i]]));
    scene->addItem(statePixmapItems[i]);
    auto pos = smokerInfo.hatPos;
    pos.setY(pos.y() - 20);
    statePixmapItems[i]->setPos(pos);
  }

  for (size_t i = 0; i < 3; ++i) {
    if (localIsSmoking[i] == SMOKING) {
      auto smokerInfo = smokers[i];
      smokersPixmapItems[i] =
          new QGraphicsPixmapItem(pixmaps[smokerInfo.missed.type]);
      scene->addItem(smokersPixmapItems[i]);
      smokersPixmapItems[i]->setPos(smokerInfo.missed.pos);
    }
  }
}
