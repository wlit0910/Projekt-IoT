#ifndef MODELVIEWPANEL_H
#define MODELVIEWPANEL_H

#include "panel.h"
#include "model.h"

class ModelViewPanel : public Panel
{
public:
    ModelViewPanel(const std::string& name);
    ~ModelViewPanel();
    void Update();
    void Draw();
    void OnSizeChanged();

    void SetModel(const std::string& path);

private:
    Model* model;
    float zoom;
};

#endif // MODELVIEWPANEL_H
