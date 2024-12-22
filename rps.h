#ifndef RPS_H
#define RPS_H
 enum RPS_CHOICE
 {
    stone = 1 ,
    paper,
    scissor
 };

 const char* choices[] = {
    "",
    [stone] = "stone",
    [paper] = "paper",
    [scissor] = "scissor",
 };
 
#endif //RPS_H