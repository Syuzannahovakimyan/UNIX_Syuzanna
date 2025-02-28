#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void rectangle_area(double , double );
void triangle_area(double, double);

int main() {
    double width_rect = 5;  
    double height_rect = 4;
    double base_tri = 8; 
    double height_tri = 3;
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    } else if (pid == 0) {
        triangle_area(base_tri,height_tri);
    } else {
        wait(NULL); 
        rectangle_area(width_rect,  height_rect);
    }

    return 0;
}

void rectangle_area(double width_rect, double height_rect) {
    double area = width_rect * height_rect;
    printf("Rectangle area: %.2f\n", area);
}

void triangle_area(double base_tri, double height_tri) {
    double area = 0.5 * base_tri * height_tri;
    printf("Triangle area: %.2f\n", area);
}
