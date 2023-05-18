#pragma once

#include "svg.h"
#include "json.h"
#include "transport_catalogue.h"
#include "domain.h"

namespace renderer {

class MapRenderer {
public:
    MapRenderer(json::Document settings, const Catalogue::TransportCatalogue& ts);

    void Render(std::ostream& out) const;

private:
    json::Document render_settings;
    svg::Document map_render;
    const Catalogue::TransportCatalogue& catalogue;


    void BuildMap();

};

}

