<<<<<<< HEAD
extern void main(){

    char * video_memory = (char*)0xb8000;
    video_memory[0] = 'Q';    
    return;
=======
int main(){
    //*(char*)0xb8000 = 'Q';  
    char* video_memory = (char*)0xb8000;
    *video_memory = 'Q';
    return 0;
>>>>>>> origin/kernel-o
}