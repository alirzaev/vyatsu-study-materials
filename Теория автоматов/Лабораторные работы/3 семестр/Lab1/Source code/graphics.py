from PyQt5 import QtCore, QtWidgets, QtGui
from time import sleep
import hanoi

QRectF = QtCore.QRectF
QPointF = QtCore.QPointF
Qt = QtCore.Qt


class RectItemAdapter(QtCore.QObject):
    def __init__(self, item: QtWidgets.QGraphicsRectItem, parent=None):
        super(RectItemAdapter, self).__init__(parent)
        self._item = item

    @QtCore.pyqtProperty(QRectF)
    def geometry(self):
        return self._item.rect()

    @geometry.getter
    def geometry(self):
        return self._item.rect()

    @geometry.setter
    def geometry(self, rect: QRectF):
        self._item.setRect(rect)


class RectItemAnimated(QtWidgets.QGraphicsRectItem):
    def __init__(self, *args):
        super(RectItemAnimated, self).__init__(*args)
        self.setPen(QtGui.QPen(Qt.NoPen))
        self.setBrush(QtGui.QBrush(QtGui.QColor('Blue')))
        self.adapter = RectItemAdapter(self, None)

    def set_color(self, color: int):
        self.setBrush(QtGui.QBrush(QtGui.QColor(color)))


class PositioningAlgo:
    def __init__(self, height: int, width: int, distance: int, base: tuple):
        self.height = height
        self.width = width
        self.distance = distance
        self.base = base

    def ring_in_tower(self, tower: int, ring: int, i: int) -> tuple:
        w = self.width * ring / 12
        h = self.height / 12
        x = (self.width - w) / 2 + self.base[0] + (self.width + self.distance) * (tower - 1)
        y = self.height * (12 - i) / 12 + self.base[1] + 2 * h

        return tuple(round(v) for v in (x, y, w, h))

    def ring_over_tower(self, tower: int, ring: int, i: int) -> tuple:
        x, y, w, h = self.ring_in_tower(tower, ring, i)
        y = self.base[1]

        return x, y, w, h

    def tower(self, i: int):
        x, y, w, h = self.ring_in_tower(i, 1, 13)
        h *= 13
        y += 5
        w -= 6
        x += 3
        return x, y, w, h

    def _next_pos_step(self, start: int, end: int, pos: int, step: int) -> int:
        sign = -1 if end - start < 0 else 1
        n = abs(pos - start) // step + 1
        pos2 = sign * step * n + start
        return pos2 if sign * (end - pos2) > 0 else end

    def _distance(self, points: tuple) -> int:
        p1, p2, p3, p4 = points
        return abs(p2[1] - p1[1]) + abs(p3[0] - p2[0]) + abs(p4[1] - p3[1])

    def next_position(self, points: tuple, rect: tuple, fps: int) -> tuple:
        pos = list(rect)
        s, t1, t2, e = points
        if 0 == fps:
            if all(i == j for i, j in zip(e, pos)):
                return [-1] * 4
            else:
                return e
        fps = min(50, fps)
        step = max(round(self._distance(points) / fps), 1)
        if pos[0] == s[0] and pos[1] > t1[1]:
            pos[1] = self._next_pos_step(s[1], t1[1], pos[1], step)
        elif pos[0] == e[0] and pos[1] < e[1]:
            pos[1] = self._next_pos_step(t2[1], e[1], pos[1], step)
        elif pos[0] == e[0] and pos[1] >= e[1]:
            pos = [-1] * 4
        else:
            pos[0] = self._next_pos_step(t1[0], t2[0], pos[0], step)
        return tuple(pos)

    def has_next_position(self, points: tuple, rect: tuple, fps: int) -> bool:
        return all(i != -1 for i in self.next_position(points, rect, fps))


class StateMachine:
    _MOVE_STATE = 0
    _GET_TOWER_STATE = 1
    _END = 2

    def __init__(self, positioning: PositioningAlgo, rings_order: list):
        self._positioning = positioning
        self._state = self._GET_TOWER_STATE
        self._towers = hanoi.HanoiTowersShuffled()
        self._points = tuple()
        self._current_position = tuple()
        self._current_ring = 0
        self.reset(rings_order)

    towers = property(lambda self: self._towers)

    def next(self, fps: int) -> tuple:
        if self._state == self._GET_TOWER_STATE:
            f, s, r = self._towers.next()
            self._points = (
                self._positioning.ring_in_tower(f, r, self._towers.height(f) + 1),
                self._positioning.ring_over_tower(f, r, 1),
                self._positioning.ring_over_tower(s, r, 1),
                self._positioning.ring_in_tower(s, r, self._towers.height(s))
            )
            self._current_ring = r
            self._current_position = self._points[0]
            self._state = self._MOVE_STATE
        if self._state == self._MOVE_STATE:
            self._current_position = self._positioning.next_position(self._points, self._current_position, fps)
            if not self._positioning.has_next_position(self._points, self._current_position, fps):
                self._state = self._GET_TOWER_STATE if self._towers.has_next() else self._END

        return self._current_ring, self._current_position

    def has_next(self) -> bool:
        return self._state != self._END

    def reset(self, rings_order=None):
        self._state = self._GET_TOWER_STATE
        self._towers.reset(rings_order)


class HanoiScene(QtWidgets.QGraphicsScene):
    timer_stopped = QtCore.pyqtSignal()

    def __init__(self, x, y, width, height, parent, settings):
        from threading import Lock
        super(HanoiScene, self).__init__(x, y, width, height, parent)
        self._positioning = PositioningAlgo(250, 150, 50, (x + 50, y + 50))
        self._settings = settings
        self._state_machine = StateMachine(self._positioning, self._settings['order'])
        self._fps = 350
        self._period = 0
        self._lock = Lock()
        self._timer = QtCore.QTimer(self)
        self._timer.setInterval(15)
        self._timer.timeout.connect(self.next)
        self._build()
        self.set_fps(self._settings['speed'])

    def _build(self):
        self._rings = [None] * 12
        self._towers = [None] * 3
        self.clear()
        for t in [1, 2, 3]:
            g = self._positioning.tower(t)
            self._towers[t - 1] = RectItemAnimated(*g)
            self._towers[t - 1].set_color(0x3f3f3f)
            self.addItem(self._towers[t - 1])

        colors = self._settings['rings']
        for i, r in enumerate(self._state_machine.towers[1]):
            g = self._positioning.ring_in_tower(1, r, i + 1)
            self._rings[r - 1] = RectItemAnimated(*g)
            self._rings[r - 1].set_color(colors[r - 1])
            self.addItem(self._rings[r - 1])

    def set_fps(self, period: int):
        self._period = period
        self._fps = int(period / self._timer.interval() / 1000)

    def set_settings(self, settings: dict):
        self._settings = settings
        self.update_colors()
        self.set_fps(self._settings['speed'])
        self.reset_order(self._settings['order'])

    fps = property(lambda self: self._fps, set_fps)

    settings = property(lambda self: self._settings, set_settings)

    def reset_order(self, rings_order=None):
        self._state_machine.reset(rings_order)

        animation_group = QtCore.QParallelAnimationGroup(self)
        for i, r in enumerate(self._state_machine.towers[1], 1):
            start = QRectF(self._rings[r - 1].rect())
            end = QRectF(*self._positioning.ring_in_tower(1, r, i))
            animation = QtCore.QPropertyAnimation(self._rings[r - 1].adapter, b'geometry', self)
            animation.setStartValue(start)
            animation.setEndValue(end)
            animation.setDuration(400)
            animation_group.addAnimation(animation)
        animation_group.start()

    def next(self):
        s = int(self._timer.interval() * 1000 / self._period)
        for _ in range(max(1, s)):
            if self._state_machine.has_next():
                ring, rect = self._state_machine.next(self.fps)
                assert all(i != -1 for i in rect)
                self._rings[ring - 1].setRect(*rect)
            else:
                self._timer.stop()
                self.timer_stopped.emit()
                break

    def start(self):
        # self._state_machine.reset_order()
        self._timer.start()

    def stop(self):
        self._timer.stop()

    def pause(self):
        self._timer.stop()

    def resume(self):
        self._timer.start()

    def shuffle(self):
        from random import shuffle
        new_order = list((range(12, 0, -1)))
        shuffle(new_order)
        self._settings['order'] = new_order
        self.reset_order(new_order)

    def sort(self):
        self._settings['order'] = list(range(12, 0, -1))
        self.reset_order(list(range(12, 0, -1)))

    def update_colors(self):
        for r, c in enumerate(self._settings['rings']):
            self._rings[r].set_color(c)
