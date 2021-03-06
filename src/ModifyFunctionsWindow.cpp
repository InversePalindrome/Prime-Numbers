/*
Copyright (c) 2017 InversePalindrome
InPal - ModifyFunctionsWindow.cpp
InversePalindrome.com
*/


#include "ModifyFunctionsWindow.hpp"

#include <wx/scrolwin.h>

#include <boost/algorithm/string.hpp>


ModifyFunctionsWindow::ModifyFunctionsWindow(wxWindow* parent, MathDataDefault* mathData) :
    wxMiniFrame(parent, wxID_ANY, "Modify Functions", wxDefaultPosition, wxDefaultSize, wxCAPTION | wxCLOSE_BOX),
    mathData(mathData),
    mainPanel(new wxPanel(this)),
    secondaryPanel(new wxPanel(this)),
    mainSizer(new wxBoxSizer(wxVERTICAL)),
    functionName(new wxStaticText(secondaryPanel, wxID_ANY, "")),
    parametersEntry(new wxTextCtrl(secondaryPanel, wxID_ANY)),
    bodyEntry(new wxTextCtrl(secondaryPanel, wxID_ANY, "", wxDefaultPosition, wxSize(250u, 300u), wxTE_MULTILINE))
{
    SetBackgroundColour(wxColor(128u, 128u, 128u));

    secondaryPanel->Hide();
    mainPanel->Show();

    auto* scrollPanel = new wxScrolledWindow(mainPanel, wxID_ANY, wxDefaultPosition, wxSize(200u, 300u));

    auto* nameText = new wxStaticText(scrollPanel, wxID_ANY, "Name");

    nameText->SetFont(wxFont(12u, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

    auto* topSizer = new wxBoxSizer(wxVERTICAL);

    topSizer->Add(nameText, 0u, wxALIGN_CENTER | wxALL, 5u);

    for (const auto& function : mathData->functions)
    {
        auto* functionButton = new wxButton(scrollPanel, wxID_ANY, function.first);

        topSizer->Add(functionButton, 0u, wxALIGN_CENTER | wxALL, 5u);

        functionButton->Bind(wxEVT_LEFT_DOWN, &ModifyFunctionsWindow::OnSelectFunction, this);
    }

    scrollPanel->SetSizer(topSizer);

    scrollPanel->FitInside();
    scrollPanel->SetScrollRate(10u, 10u);

    topSizer->Fit(this);
    topSizer->SetSizeHints(this);

    mainSizer->Add(mainPanel, 0u, wxEXPAND | wxALL);

    mainSizer->Fit(this);
    mainSizer->SetSizeHints(this);

    SetSizer(mainSizer);
}

void ModifyFunctionsWindow::OnSelectFunction(wxMouseEvent & event)
{
    this->mainPanel->Hide();
    this->secondaryPanel->Show();

    auto* selectedButton = dynamic_cast<wxButton*>(event.GetEventObject());

    auto* nameText = new wxStaticText(secondaryPanel, wxID_ANY, "Name");

    this->functionName->SetLabel(selectedButton->GetLabel().ToStdString());

    auto* parametersText = new wxStaticText(secondaryPanel, wxID_ANY, "Parameters");

    this->parametersEntry->SetValue(this->mathData->functions.at(selectedButton->GetLabel().ToStdString()).first);

    auto* bodyText = new wxStaticText(secondaryPanel, wxID_ANY, "Body");

    this->bodyEntry->SetValue(this->mathData->functions.at(selectedButton->GetLabel().ToStdString()).second);

    auto* modifyButton = new wxButton(secondaryPanel, wxID_ANY, "Modify");
    auto* deleteButton = new wxButton(secondaryPanel, wxID_DELETE);

    auto* buttonSizer = new wxBoxSizer(wxHORIZONTAL);

    buttonSizer->Add(modifyButton, 0u, wxALIGN_CENTER | wxALL, 5u);
    buttonSizer->Add(deleteButton, 0u, wxALIGN_CENTER | wxALL, 5u);

    auto* bottomSizer = new wxBoxSizer(wxVERTICAL);

    bottomSizer->Add(nameText, 0u, wxALL, 3u);
    bottomSizer->Add(functionName, 0u, wxEXPAND | wxALL, 5u);
    bottomSizer->Add(parametersText, 0u, wxALL, 5u);
    bottomSizer->Add(parametersEntry, 0u, wxEXPAND | wxALL, 5u);
    bottomSizer->Add(bodyText, 0u, wxALL, 5u);
    bottomSizer->Add(bodyEntry, 0u, wxALL, 5u);
    bottomSizer->Add(buttonSizer, 0u, wxALIGN_CENTER | wxALL, 5u);

    this->secondaryPanel->SetSizer(bottomSizer);

    auto& font = wxFont(8u, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);

    nameText->SetFont(font);
    parametersText->SetFont(font);
    bodyText->SetFont(font);

    this->mainSizer->Add(secondaryPanel);

    this->secondaryPanel->FitInside();

    this->mainSizer->Fit(this);
    this->mainSizer->SetSizeHints(this);

    modifyButton->Bind(wxEVT_LEFT_DOWN, &ModifyFunctionsWindow::OnModifyFunction, this);
    deleteButton->Bind(wxEVT_LEFT_DOWN, &ModifyFunctionsWindow::OnDeleteFunction, this);
}

void ModifyFunctionsWindow::OnModifyFunction(wxMouseEvent & event)
{
    std::vector<std::string> parameterTokens;

    boost::split(parameterTokens, this->parametersEntry->GetValue().ToStdString(), boost::is_any_of(", "));

    this->mathData->mathSolver.addCompositorFunction(this->functionName->GetLabel().ToStdString(), parameterTokens, this->bodyEntry->GetValue().ToStdString());

    this->mathData->functions.at(this->functionName->GetLabel().ToStdString()) =
        std::make_pair(this->parametersEntry->GetValue().ToStdString(), this->bodyEntry->GetValue().ToStdString());

    this->Close();
}

void ModifyFunctionsWindow::OnDeleteFunction(wxMouseEvent & event)
{
    this->mathData->mathSolver.removeFunction(this->functionName->GetLabel().ToStdString());
    this->mathData->functions.erase(this->functionName->GetLabel().ToStdString());

    this->Close();
}