#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>
#include <algorithm>

using namespace std;

// Перечислимый тип для статуса задачи
enum class TaskStatus {
    NEW,          // новая
    IN_PROGRESS,  // в разработке
    TESTING,      // на тестировании
    DONE          // завершена
};

// Объявляем тип-синоним для map<TaskStatus, int>,
// позволяющего хранить количество задач каждого статуса
using TasksInfo = map<TaskStatus, int>;

class TeamTasks {
public:

//     void PrintTasksInfoClass(TasksInfo tasks_info) {
//     cout << tasks_info[TaskStatus::NEW] << " new tasks"s
//          << ", "s << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress"s
//          << ", "s << tasks_info[TaskStatus::TESTING] << " tasks are being tested"s
//          << ", "s << tasks_info[TaskStatus::DONE] << " tasks are done"s << endl;
// }


    // Получить статистику по статусам задач конкретного разработчика
    const TasksInfo& GetPersonTasksInfo(const string& person) const{
        return person_task_info_.at(person);
    }

    // Добавить новую задачу (в статусе NEW) для конкретного разработчитка
    void AddNewTask(const string& person){
        ++person_task_info_[person][TaskStatus::NEW];
    }

    // Обновить статусы по данному количеству задач конкретного разработчика,
    // подробности см. ниже
    tuple<TasksInfo, TasksInfo> PerformPersonTasks(const string& person, int control){

        if(!person_task_info_.count(person)) { 
            return {{}, {}};
        }
        TasksInfo from_Update_tasks;
        TasksInfo from_Unfulfillied_tasks;
        
        TasksInfo task_from_transfer;
        
        int task_count = control;
        for(int index = 0; index < 3; ++index)
        {
            int counts = person_task_info_[person][static_cast<TaskStatus>(index)];
            if((!counts)) { continue; }

            if(task_count > counts)
            {
                task_from_transfer[static_cast<TaskStatus>(index + 1)] = counts;
                task_count -= counts;
            }
            else
            {
                task_from_transfer[static_cast<TaskStatus>(index + 1)] = task_count;
                task_count = 0;
            }
        }
        
        for(int index = 1; index < 4; ++index)
        {
            TaskStatus last = static_cast<TaskStatus>(index - 1);
            TaskStatus now = static_cast<TaskStatus>(index);
            int TASK_NOW = task_from_transfer[now];
            person_task_info_[person][last] -= TASK_NOW;
            person_task_info_[person][now] += TASK_NOW;
            from_Unfulfillied_tasks[last] = 
                                person_task_info_[person][last] - task_from_transfer[last];
            from_Update_tasks[now] = TASK_NOW;
        }

        from_Unfulfillied_tasks.erase(TaskStatus::DONE);
        return make_tuple(from_Update_tasks, from_Unfulfillied_tasks);
    }

private:
    map<string, TasksInfo> person_task_info_;
};

// Принимаем словарь по значению, чтобы иметь возможность
// обращаться к отсутствующим ключам с помощью [] и получать 0,
// не меняя при этом исходный словарь.
void PrintTasksInfo(TasksInfo tasks_info) {
    cout << tasks_info[TaskStatus::NEW] << " new tasks"s
         << ", "s << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress"s
         << ", "s << tasks_info[TaskStatus::TESTING] << " tasks are being tested"s
         << ", "s << tasks_info[TaskStatus::DONE] << " tasks are done"s << endl;
}

int main() {
    TeamTasks tasks;
    tasks.AddNewTask("Ilia"s);
    for (int i = 0; i < 3; ++i) {
        tasks.AddNewTask("Ivan"s);
    }
    cout << "Ilia's tasks: "s;
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"s));
    cout << "Ivan's tasks: "s;
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"s));

    TasksInfo updated_tasks, untouched_tasks;

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan"s, 2);
    cout << "Updated Ivan's tasks: "s;
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: "s;
    PrintTasksInfo(untouched_tasks);

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan"s, 2);
    cout << "Updated Ivan's tasks: "s;
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: "s;
    PrintTasksInfo(untouched_tasks);
}