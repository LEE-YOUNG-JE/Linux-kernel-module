#include <linux/init_task.h> //init_task를 위해서 따로 include한 부분
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
/* performs a depth-first traversal of the list of tasks in the system. */
void traverse(struct task_struct *ptr) { //ptr이 init_task를 받아오므로 init_task와 같은 형식으로 구현
/* fill in your code */
struct task_struct *task; //init_task를 받은 ptr다음으로 어떤 task들을 구조체로 지니는지 알아야하므로 따로 선언
struct list_head *list; //마찬가지로 task의 head부분이 어디인지 가르키기 위해 따로 선언
list_for_each(list, &ptr->children){ //list가 매 반복문마다 위치를 지정해주고 &ptr->children이 head이다. 
task = list_entry(list, struct task_struct, sibling); //task가 head의 모든 자식을 가르키는 구조체이다. 
printk(KERN_INFO "pid = %d \tname = %s ",task->pid,task->comm);
if(task!=NULL){ //task가 NULL이 아니라면 더 있으므로 재귀를 통해서 dfs를 실행한다. 
traverse(task);
}
}
}
int simple_init(void)
{
 printk(KERN_INFO "Loading Module\n");
traverse(&init_task);
 return 0;
}
void simple_exit(void) {
printk(KERN_INFO "Removing Module\n");
struct task_struct *queue[1000]; //bfs를 실행해야 하므로 queue를 활용, 넉넉하게 1000개를 사용하고 task와 같은 type으로 선언한다. int i = 0; //queue의 인덱스로 사용
int j = 0; //queue에 있는 값을 출력하기 위한 반복문에서 사용
int start = 0; //첫 시작이 어디인지 가르키는 변수
struct task_struct *task;
struct list_head *list;
task = &init_task; //첫 시작을 init_task의 주소로 받음
queue[i] = task; //첫 0번 pid의 주소를 queue배열에 넣음
while(queue[start]){//start에 값이 있다면 그 start의 자식을 다시 배열에 넣어야 하므로 계속 반복
list_for_each(list,&queue[start]->children){ //queue의 children이 head이고 list가 그 children을 지시
queue[++i]=list_entry(list, struct task_struct, sibling); //그 멤버변수 구조체를 queue에 삽입
}
start++; //start를 증가시켜서 있는지 queue에 있는지 확인
}
for(j =0; queue[j]!=NULL; j++){ //queue에 값이 없을 때까지 queue의 pid와 comm를 출력
printk(KERN_INFO "pid = %d \tname = %s ",queue[j]->pid,queue[j]->comm);
}
}
module_init( simple_init );
module_exit( simple_exit );
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Process Module");
MODULE_AUTHOR("SGG")