extern "C" int main(){

    *(char*)0xb8000 = 'Q';    

    while(true) ;
    return 0;
}