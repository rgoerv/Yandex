#include "json_reader.h"
#include "map_renderer.h"

#include <iostream>

int main() 
{
    /*
     * Примерная структура программы:
     *
     * Считать JSON из stdin
     * Построить на его основе JSON базу данных транспортного справочника
     * Выполнить запросы к справочнику, находящиеся в массиве "stat_requests", построив JSON-массив
     * с ответами.
     * Вывести в stdout ответы в виде JSON
     */
    JsonReader::Reader reader(std::cin);
    renderer::MapRenderer renderer(reader.GetRenderSettings(), reader.GetCatalogue());
    renderer.Render(std::cout);
    // reader.Reply(std::cout);
    // std::cout << "success";

}