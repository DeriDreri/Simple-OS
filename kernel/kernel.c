int main(){
    //*(char*)0xb8000 = 'Q';  
    char* video_memory = (char*)0xb8000;
    *video_memory = 'Q';
    return 0;
}