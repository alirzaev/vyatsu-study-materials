#include <wx/wx.h>
#include <wx/valnum.h>

#include <map>
#include <memory>

#include "conductors.h"

using fpvalidator = wxFloatingPointValidator<double>;
using stodmap = std::map<wxString, double>;

class MetalSizer : public wxGridSizer {
private:
	BaseConductor* conductor;
	stodmap resistivitymp;
	wxComboBox* resistivityBox;
	wxTextCtrl* lengthEntry;
	wxTextCtrl* surfaceEntry;
	wxButton* compute;

	void OnComputeClick(wxCommandEvent& event) {
		double val;
		bool ok;
		ok = lengthEntry->GetValue().ToDouble(&val);
		conductor->setLength(ok ? val : 0);

		conductor->setResistivity(resistivitymp.at(resistivityBox->GetStringSelection()));

		ok = surfaceEntry->GetValue().ToDouble(&val);
		dynamic_cast<MetalConductor*>(conductor)->setSurface(ok ? val : 0);

		if (this->conductor->check()) {
			wxString res = wxString::FromDouble(conductor->resistance());
			wxMessageBox("Сопротивление проводника: " + res + " Ом", "Результат");
		}
		else {
			wxMessageBox("Некорректные данные", "Ошибка", wxICON_ERROR);
		}
	}
public:
	MetalSizer(wxWindow* parent, const stodmap& mr) : wxGridSizer(4, 2, 0, 0), resistivitymp(mr), conductor(new MetalConductor()) {
		wxSizerFlags textFlags = wxSizerFlags().Left();
		wxSizerFlags boxFlags = wxSizerFlags().Top().Right();

		wxStaticText* resistivity = new wxStaticText(parent, wxID_ANY, "Удельное сопротивление\n(Ом*мм^2/м)");
		wxStaticText* length = new wxStaticText(parent, wxID_ANY, "Длина (м)");
		wxStaticText* surface = new wxStaticText(parent, wxID_ANY, "Площадь сечения (мм^2)");
		
		resistivity->SetExtraStyle(wxALIGN_LEFT);
		length->SetExtraStyle(wxALIGN_LEFT);
		surface->SetExtraStyle(wxALIGN_LEFT);

		resistivityBox = new wxComboBox(parent, wxID_ANY);
		lengthEntry = new wxTextCtrl(parent, wxID_ANY);
		surfaceEntry = new wxTextCtrl(parent, wxID_ANY);

		lengthEntry->SetValidator(fpvalidator(3, nullptr, wxNUM_VAL_NO_TRAILING_ZEROES));
		surfaceEntry->SetValidator(fpvalidator(3, nullptr, wxNUM_VAL_NO_TRAILING_ZEROES));

		resistivityBox->SetEditable(false);
		for (const auto& p : resistivitymp) {
			resistivityBox->AppendString(p.first);
		}
		resistivityBox->SetSelection(0);

		compute = new wxButton(parent, wxID_ANY, "Вычислить");
		compute->Bind(wxEVT_BUTTON, &MetalSizer::OnComputeClick, this);

		this->Add(resistivity, textFlags);
		this->Add(resistivityBox, boxFlags);
		this->Add(length, textFlags);
		this->Add(lengthEntry, boxFlags);
		this->Add(surface, textFlags);
		this->Add(surfaceEntry, boxFlags);
		this->Add(compute, boxFlags);
	}
};

class LiquidSizer : public wxGridSizer {
private:
	BaseConductor* conductor;
	stodmap resistivitymp;
	wxComboBox* resistivityBox;
	wxTextCtrl* lengthEntry;
	wxTextCtrl* widthEntry;
	wxTextCtrl* heightEntry;
	
	void OnComputeClick(wxCommandEvent& event) {
		double val;
		bool ok;
		ok = lengthEntry->GetValue().ToDouble(&val);
		conductor->setLength(ok ? val : 0);

		ok = widthEntry->GetValue().ToDouble(&val);
		dynamic_cast<LiquidConductor*>(conductor)->setWidth(ok ? val : 0);

		ok = heightEntry->GetValue().ToDouble(&val);
		dynamic_cast<LiquidConductor*>(conductor)->setHeight(ok ? val : 0);

		conductor->setResistivity(resistivitymp.at(resistivityBox->GetStringSelection()));

		if (this->conductor->check()) {
			wxString res = wxString::FromDouble(conductor->resistance());
			wxMessageBox("Сопротивление проводника: " + res + " Ом", "Результат");
		}
		else {
			wxMessageBox("Некорректные данные", "Ошибка", wxICON_ERROR);
		}
	}
public:
	LiquidSizer(wxWindow* parent, const stodmap& lr) : wxGridSizer(5, 2, 0, 0), resistivitymp(lr), conductor(new LiquidConductor()) {
		wxSizerFlags textFlags = wxSizerFlags().Left();
		wxSizerFlags boxFlags = wxSizerFlags().Top().Right();

		wxStaticText* resistivity = new wxStaticText(parent, wxID_ANY, "Удельное сопротивление\n(Ом*мм^2/м)");
		wxStaticText* length = new wxStaticText(parent, wxID_ANY, "Длина (м)");
		wxStaticText* width = new wxStaticText(parent, wxID_ANY, "Ширина (м)");
		wxStaticText* height = new wxStaticText(parent, wxID_ANY, "Высота (м)");
		for (wxStaticText* text : { resistivity, length, width, height }) {
			text->SetExtraStyle(wxALIGN_LEFT);
		}

		resistivityBox = new wxComboBox(parent, wxID_ANY);
		lengthEntry = new wxTextCtrl(parent, wxID_ANY);
		widthEntry = new wxTextCtrl(parent, wxID_ANY);
		heightEntry = new wxTextCtrl(parent, wxID_ANY);
		
		lengthEntry->SetValidator(fpvalidator(3, nullptr, wxNUM_VAL_NO_TRAILING_ZEROES));
		widthEntry->SetValidator(fpvalidator(3, nullptr, wxNUM_VAL_NO_TRAILING_ZEROES));
		heightEntry->SetValidator(fpvalidator(3, nullptr, wxNUM_VAL_NO_TRAILING_ZEROES));

		resistivityBox->SetEditable(false);
		for (const auto& p : resistivitymp) {
			resistivityBox->AppendString(p.first);
		}
		resistivityBox->SetSelection(0);

		wxButton* compute = new wxButton(parent, wxID_ANY, "Вычислить");
		compute->Bind(wxEVT_BUTTON, &LiquidSizer::OnComputeClick, this);

		this->Add(resistivity, textFlags);
		this->Add(resistivityBox, boxFlags);
		this->Add(length, textFlags);
		this->Add(lengthEntry, boxFlags);
		this->Add(width, textFlags);
		this->Add(widthEntry, boxFlags);
		this->Add(height, textFlags);
		this->Add(heightEntry, boxFlags);
		this->Add(compute, boxFlags);
	}
};

class MainFrame : public wxFrame {
public:
	MainFrame() : wxFrame(NULL, wxID_ANY, "Lab6") {
		stodmap mr = {
			{ "Алюминий", 0.028  },
			{ "Бронза",   0.1    },
			{ "Железо",   0.1    },
			{ "Золото",   0.023  },
			{ "Латунь",   0.025  },
			{ "Медь",     0.0175 },
			{ "Олово",    0.12   },
			{ "Платина",  0.107  },
			{ "Свинец",   0.22   },
			{ "Серебро",  0.015  },
			{ "Цинк",     0.054  },
			{ "Ртуть",    0.94   },
			{ "Вольфрам", 0.05   }
		};
		stodmap lr = {
			{ "Гидроксид натрия ", 0.032 },
			{ "Медный купорос",    0.315 },
			{ "Серная кислота",    0.025 },
			{ "Соляная кислота",   0.016 },
			{ "Хлорид натрия",     0.083 }
		};

		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		wxSizerFlags flags = wxSizerFlags().Expand().Border();

		sizer->Add(new MetalSizer(this, mr), flags);
		sizer->Add(new LiquidSizer(this, lr), flags);

		this->SetSizer(sizer);
		this->SetSize(wxSize(600, 300));
		this->SetBackgroundColour(wxColor("White"));
	}
};

class Lab6App : public wxApp {
public:
	virtual bool OnInit() {
		MainFrame *frame = new MainFrame();
		frame->Show(true);
		return true;
	}
};

wxIMPLEMENT_APP(Lab6App);
