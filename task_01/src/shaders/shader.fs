#version 330 core

out vec4 fragColor;

uniform vec2  iResolution;
uniform float iTime;

uniform vec3  camera_position;
uniform mat4  transform_matrix;


///////////////////////////////////////////////////////////////
///                                                         ///
///                   Важные константы                      ///
///                                                         ///
///////////////////////////////////////////////////////////////

/* Константы */
float EPS        = 0.01;                  // Эпслилон пересечений
float MAX_VIEW   = 500.;                  // Максимальный обзор
vec3  STD_BRIGHT = vec3(0.1, 0.1, 0.1);   // Яркость дневного цвета
float SHADOW     = 10.;                   // Твёрдость тени
float MIN_STEP   = 0.01;                  // Минимальный шаг



///////////////////////////////////////////////////////////////
///                                                         ///
///                Фигура. Работа с фигурами.               ///
///                                                         ///
///////////////////////////////////////////////////////////////

const int FIGURE_NUMBER = 5; // Количество фигур

/* Определение класса фигуры и массива figures */
const int FIG_SPHERE     = 1;
const int FIG_UDROUNDBOX = 2;
const int FIG_PLANE      = 3;
const int FIG_ELLIPSE    = 4;
const int FIG_TORUS      = 5;

struct Figure {
    int  type;  // Тип фигуры
    vec3 point; // Точка фигуры
    vec3 color; // Цвет фигуры
    
    // koeffs.x - specular reflection constant
    // koeffs.y - diffuse reflection constant
    // koeffs.z - ambient reflection constant
    // koeffs.w - shininess constant
    // Считаем, что коэффициенты для R, G и В равны, потому что лень придумывать случаи, когда это не так.
    // И что у всей фигуры эи коэффициенты постоянны -- поэтому же.
    vec4 koeffs;
    float reflection; // Коэффициент отражения 
};
Figure[FIGURE_NUMBER] figures;

/* Цвет фигуры */
vec3
getFigureColor(vec3 point,
               int  fig)
{
    Figure figure = figures[fig];
    switch (figure.type) {
    case FIG_SPHERE:
    case FIG_UDROUNDBOX:
    case FIG_ELLIPSE:
    case FIG_TORUS:
        return figure.color;
    case FIG_PLANE:
        float x = mod(point.x, 20.);
        float z = mod(point.z, 20.);
        if ((x > 10. && z > 10.) || (x < 10. && z < 10.)) {
        	return figure.color;
        } else {
        	return 0.8 * figure.color;
        }
    }
}

/* Расстояние до фигуры */
float
getDist (int  fig,
         vec3 point)
{
    Figure figure = figures[fig];
	vec3 p = figure.point - point;
    
    switch (figure.type) {
   	case FIG_SPHERE:
    	float radius = 5.;
    
    	return length(p) - radius;
    
    case FIG_UDROUNDBOX:
        vec3 b = vec3(3., 3.*sin(iTime) + 4., 3.);
    	float r = 1.;
  	
    	return length(max(abs(p)-b,0.0))-r;
        
    case FIG_PLANE:
  		vec3 bb = vec3(500., 1., 500.);
        
        vec3 d = abs(p) - bb;
  		return min(max(d.x,max(d.y,d.z)),0.0) + length(max(d,0.0));
	
    case FIG_ELLIPSE:
        vec3 rr = vec3(10., 15., 20.);
        vec3 cc = vec3(1., 3., 8.);
        float ellipse = (length( p/rr ) - 1.0) * min(min(rr.x,rr.y),rr.z);
    	float cilindr = length(p.xz-cc.xy)-cc.z;
        return max(-cilindr, ellipse);
    
    case FIG_TORUS:
        vec2 tt = vec2(15., 2.);
        vec2 q = vec2(length(p.xz)-tt.x,p.y);
  		float torus = length(q)-tt.y;
        float problem = sin(100.*p.x)*sin(100.*p.y)*sin(100.*p.z);
        return torus + problem;
    }
}

/* Нормаль фигуры в точке */
vec3
getNormal (int  fig,
           vec3 point)
{
	vec3 z1 = point + vec3(EPS, 0, 0);
 	vec3 z2 = point - vec3(EPS, 0, 0);
 	vec3 z3 = point + vec3(0, EPS, 0);
 	vec3 z4 = point - vec3(0, EPS, 0);
 	vec3 z5 = point + vec3(0, 0, EPS);
 	vec3 z6 = point - vec3(0, 0, EPS);
 	float dx = getDist(fig, z1) - getDist(fig, z2);
 	float dy = getDist(fig, z3) - getDist(fig, z4);
 	float dz = getDist(fig, z5) - getDist(fig, z6);
 	return normalize(vec3(dx, dy, dz));
}

/* Самая близкая фигура и расстояие до неё */
struct Dist 
{
	float dist;
    int   fig;
};
Dist
getMinDist (vec3 point)
{
	float min_dist = getDist(0, point);
    int   fig      = 0;
    for (int cur_fig = 1; cur_fig < FIGURE_NUMBER; ++cur_fig) {
        float dist = getDist(cur_fig, point);
        if (dist < min_dist) {
         	min_dist = dist;
            fig      = cur_fig; 
        }
    }
    Dist result;
    result.dist = min_dist;
    result.fig  = fig;
    return result;
}
    


///////////////////////////////////////////////////////////////
///                                                         ///
///                    Источник света.                      ///
///                                                         ///
///////////////////////////////////////////////////////////////

const int LIGHT_NUMBER = 3;

/* Определение класса источника света и массива lights */
struct Light {
    vec3 point;
    vec3 bright;
};
Light[LIGHT_NUMBER] lights;


///////////////////////////////////////////////////////////////
///                                                         ///
///                 Пересечение с фигурой.                  ///
///                                                         ///
///////////////////////////////////////////////////////////////


/* Структура пересечения */
struct Hit
{
    bool  is_hit;  // Было ли попадание в фигуру
	vec3  point;   // Точка попадания
    int   fig;     // Фигура, в которую попали
    float dist;    // Расстояние до фигуры
};

/* Поиск пересечения */
Hit
getHit (vec3 ray_point,
        vec3 ray_dir)
{
    Hit result;
    while (true) {
        Dist  dist     = getMinDist(ray_point);
        float min_dist = dist.dist;
        int   fig      = dist.fig;
    	
        /* Слишком далеко ушли */
        if (length(ray_point) > MAX_VIEW) {
         	result.is_hit = false;
            result.point = ray_point;
            return result;
        }
            
        /* Попали в фигуру */
        if (min_dist < EPS) {
         	result.is_hit = true;
            result.point  = ray_point;
            result.fig    = fig;
            result.dist   = min_dist;
            return result;
        }
        /* Увеличиваем луч */
        if (min_dist < MIN_STEP) {
        	ray_point = ray_point + MIN_STEP * ray_dir;
        }
        ray_point = ray_point + min_dist * ray_dir;        
    }
}

///////////////////////////////////////////////////////////////
///                                                         ///
///                  Освещение фигуры.                      ///
///                                                         ///
///////////////////////////////////////////////////////////////

/* Интенсивность света в точке по модели Фонга */
vec3
getPhong (vec3 point,
          vec3 light_point,
          vec3 eye_point,
          vec3 bright,
          int  fig)
{
	Figure figure = figures[fig];
    
    vec3 normal_dir  = getNormal(fig, point);
    vec3 light_dir   = normalize(light_point - point);
    vec3 eye_dir     = normalize(eye_point - point);
    vec3 reflect_dir = normalize(reflect(-light_dir, normal_dir));
    
    float dotLN = dot(light_dir,  normal_dir);
    float dotRE = dot(reflect_dir, eye_dir);
    
    if (dotLN < 0.0) {
        return vec3(0.0, 0.0, 0.0);
    }     
    if (dotRE < 0.0) {
        return bright * (figure.koeffs.y * dotLN);
    }
    return bright * (figure.koeffs.y * dotLN + figure.koeffs.x * pow(dotRE, figure.koeffs.w));

}

/* Коэффициент тени */
float
getShadow (vec3 point,
           vec3 light_point)
{
    vec3  light_dir = normalize(light_point - point);    
    float maxt      = length(light_point - point) + 10.*EPS;
    float t         = 100. * EPS;
    float phi       = EPS;
    
    float result = 1.0;
    while (t < maxt)
    {
        vec3 cur_point = point + t*light_dir;
    	Dist dist = getMinDist(cur_point);
        float min_dist = dist.dist;
        
        if (min_dist < EPS) {
        	return 0.;
        }
        
        float y = min_dist*min_dist/(2.0*phi);
        float d = sqrt(min_dist*min_dist - y*y);
        result  = min(result, SHADOW*d/max(0., t - y));
        
        phi = min_dist;
        t  += (min_dist < MIN_STEP)? MIN_STEP : min_dist;
        
    }
    return result;
}

/* Цвет точки */
vec3
getColor (vec3 point, 
          vec3 eye_point, 
          vec3 color, 
          int  fig)
{
    Figure figure = figures[fig];
    
    vec3 result = figure.koeffs.x * STD_BRIGHT;
    
    for (int i = 0; i < LIGHT_NUMBER; ++i) {
        Light light = lights[i];
        float shadow = getShadow(point, light.point);
        vec3 phong  = getPhong(point, light.point, eye_point, light.bright, fig);
        result += shadow * phong / (length(point - light.point) / 80.);    
    } 
    return result * color;
}


///////////////////////////////////////////////////////////////
///                                                         ///
///                     Цвет фона.                          ///
///                                                         ///
///////////////////////////////////////////////////////////////

/* Белый шум из Интернета */
float rand2D(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

/* Белый шум из Интернета для звёзд */
float dotNoise2D(in float x, in float y, in float fractionalMaxDotSize, in float dDensity)
{
    float integer_x = x - fract(x);
    float fractional_x = x - integer_x;

    float integer_y = y - fract(y);
    float fractional_y = y - integer_y;

   	if (rand2D(vec2(integer_x+1.0, integer_y +1.0)) > dDensity)
      	{return 0.0;}

    float xoffset = (rand2D(vec2(integer_x, integer_y)) -0.5);
    float yoffset = (rand2D(vec2(integer_x+1.0, integer_y)) - 0.5);
    float dotSize = 0.5 * fractionalMaxDotSize * max(0.25,rand2D(vec2(integer_x, integer_y+1.0)));

   	vec2 truePos = vec2 (0.5 + xoffset * (1.0 - 2.0 * dotSize) , 0.5 + yoffset * (1.0 -2.0 * dotSize));

   	float distance = length(truePos - vec2(fractional_x, fractional_y));

   	return 1.0 - smoothstep (0.3 * dotSize, 1.0* dotSize, distance);

}

/* Цвет фона. Белый шум + подходящие параметры. */
float getBackgroundColor(vec2 coord)
{
    float wavelength = 0.003;
    float fractionalMaxDotSize = 100.;
    float dDensity = 0.01;
    return dotNoise2D(coord.x/wavelength, coord.y/wavelength, fractionalMaxDotSize, dDensity);
}


///////////////////////////////////////////////////////////////
///                                                         ///
///                Начальные точки и векторы.               ///
///                                                         ///
///////////////////////////////////////////////////////////////

/* Направление первых лучей */
struct StartRays
{
	vec3 point;
    vec3 dir;
    vec3 eye_point;
};
    
StartRays
getStart (float x,
          float y,
          float screen_width,
          float screen_height)
{
    StartRays start;
    
    /* Будем считать оси из центра экрана */
    float screen_x = x - screen_width / 2.0;
    float screen_y = y - screen_height / 2.0;
    
    /* Задали начальный луч */
    start.point = vec3(10.* screen_x/screen_width,
                            10.* screen_y/screen_width,
                            10.);
    start.dir   = normalize(start.point);

    start.point = (transform_matrix * vec4(start.point, 1.0)).xyz + camera_position;
    start.dir   = (transform_matrix * vec4(start.dir, 1.0)).xyz;

    start.eye_point  = camera_position;
    
    return start;
}


void main()
{
    ////////////////////////////////////////////////////////////
    ///                                                      ///
    ///            Положение фигур и источников света.       ///
    ///                                                      ///
    ////////////////////////////////////////////////////////////
    
    figures[0].type   = FIG_SPHERE;
    float v0z  = 2.;
    float v0x  = -1.;
    float v0y  = 20.;
    float g    = 9.8;
    float time = mod(iTime, 2.*v0y/g);
    figures[0].point  = vec3(15. + v0x * iTime,-4. - g * time*time/2. + v0y*time, 30. + v0z*iTime);
    figures[0].color  = vec3(1., 0., 0.);
    figures[0].koeffs = vec4(0.4, 0.8, 0.5, 3);
    figures[0].reflection = 0.;
    
    figures[1].type   = FIG_UDROUNDBOX;
    figures[1].point  = vec3(-30., 0., 80.);
    figures[1].color  = vec3(1., 1., 1.);
    figures[1].koeffs = vec4(0.2, 0.6, 0.3, 1);
    figures[1].reflection = 0.;
    
    figures[2].type   = FIG_PLANE;
    figures[2].point  = vec3(0., -10., 100.);
    figures[2].color  = vec3(0., 1., 0.);
    figures[2].koeffs = vec4(0.4, 0.3, 0.3, 6);
    figures[2].reflection = 0.1;
    
    figures[3].type   = FIG_ELLIPSE;
    figures[3].point  = vec3(30., 20., 60.);
    figures[3].color  = vec3(0., 1., 1.);
    figures[3].koeffs = vec4(0.4, 0.5, 0.3, 0.5);
    figures[3].reflection = 0.3;
    
    figures[4].type   = FIG_TORUS;
    figures[4].point  = vec3(-50., 20., 60.);
    figures[4].color  = vec3(1., 0., 1.);
    figures[4].koeffs = vec4(0.4, 0.5, 0.3, 0.5);
    figures[4].reflection = 0.;
    
    lights[0].point  = vec3(30., 100., 40.*sin(iTime));
    lights[0].bright = vec3(0.9, 0.9, 0.9);
    
    lights[1].point  = vec3(-10.*sin(iTime), 25. + 10.*sin(iTime), 90.*sin(iTime));
    lights[1].bright = vec3(0.5, 0.5, 0.5);
    
    lights[2].point  = vec3(-10., 10., 2.);
    lights[2].bright = vec3(0.9, 0.9, 0.9) * abs(sin(1. + 0.25*iTime));

    
    ////////////////////////////////////////////////////////////
    ///                                                      ///
    ///                     Алгоритм.                        ///
    ///                                                      ///
    ////////////////////////////////////////////////////////////
    
    StartRays start = getStart(gl_FragCoord.x, gl_FragCoord.y, iResolution.x, iResolution.y);
    vec3 ray_point = start.point;
    vec3 ray_dir   = start.dir;
    vec3 eye_point = start.eye_point;
    
    /* Первый луч */
    Hit hit    = getHit(ray_point, ray_dir);    
    if (!hit.is_hit) {
    	fragColor = vec4(getBackgroundColor(hit.point.xz) * vec3(1.), 1.);
        return;
    }
    
    /* Отражение */
    int  fig    = hit.fig;
    vec3 point  = hit.point;
    vec3 color = getFigureColor(point, fig);
    Figure figure = figures[fig];
    
    if (figure.reflection < EPS) {
    	color = getColor(point, eye_point, color, fig);
        fragColor = vec4(color, 1.);
        return;
    }
        
    vec3 normal      = getNormal(fig, point);
    vec3 reflect_dir = normalize(reflect(ray_dir, normal));
    
    hit = getHit(hit.point+10.*EPS*reflect_dir, reflect_dir);
    if (!hit.is_hit) {
    	color = figure.reflection * getBackgroundColor(hit.point.xz) * vec3(1.) +
            (1. - figure.reflection) * color;
    } else {
    	color = figure.reflection * getColor(hit.point, eye_point, getFigureColor(hit.point, hit.fig), hit.fig)       +      (1. - figure.reflection) * color;
    }
    
    color = getColor(point, eye_point, color, fig);
    
    fragColor = vec4(color, 1.);
       
}
