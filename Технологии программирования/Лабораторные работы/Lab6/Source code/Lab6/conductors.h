#pragma once

class BaseConductor {
private:
	double m_resistivity = 0;
	double m_length = 0;
public:
	BaseConductor() = default;

	double resistivity() const {
		return m_resistivity;
	}

	void setResistivity(double resistivity) {
		m_resistivity = resistivity;
	}

	double length() const {
		return m_length;
	}

	void setLength(double length) {
		m_length = length;
	}

	virtual double resistance() const {
		return 0;
	}

	virtual bool check() const {
		return m_resistivity > 0 && m_length > 0;
	}
};

class MetalConductor : public BaseConductor {
private:
	double m_surface = 0;
public:
	MetalConductor() = default;

	double surface() const {
		return m_surface;
	}

	void setSurface(double surface) {
		m_surface = surface;
	}

	double resistance() const override {
		return resistivity() * length() / surface();
	}

	bool check() const override {
		return BaseConductor::check() && m_surface > 0;
	}
};

class LiquidConductor : public BaseConductor {
private:
	double m_width = 0;
	double m_heigth = 0;
public:
	LiquidConductor() = default;

	double width() const {
		return m_width;
	}

	void setWidth(double width) {
		m_width = width;
	}

	double height() const {
		return m_heigth;
	}

	void setHeight(double height) {
		m_heigth = height;
	}

	double resistance() const override {
		return resistivity() * length() / (width() * height());
	}

	bool check() const override {
		return BaseConductor::check() && m_width > 0 && m_heigth > 0;
	}
};