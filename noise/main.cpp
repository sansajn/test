#include <iostream>
#include <glm/glm.hpp>
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_io.hpp>

using std::string;
using std::cout;
using glm::vec4;
using glm::vec3;
using glm::vec2;
using glm::mod;
using glm::floor;
using glm::mix;
using glm::atan;
using glm::sin;
using glm::length;
using glm::min;
using glm::max;

using namespace boost::gil;


void print(vec2 const & v, string const & name)
{
	if (!name.empty())
		cout << name << ":";
	cout << "(" << v.x << ", " << v.y << ")\n";
}

void print(vec3 const & v, string const & name)
{
	if (!name.empty())
		cout << name << ":";
	cout << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
}

void print(vec4 const & v, string const & name)
{
	if (!name.empty())
		cout << name << ":";
	cout << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")\n";
}


float snoise(vec3 uv, float res)  // uv a pritom vec3 ?
{
	vec3 const s = vec3{1e0, 1e2, 1e3};
	uv *= res;
	vec3 uv0 = floor(mod(uv, res)) * s;
	vec3 uv1 = floor(mod(uv+vec3{1}, res)) * s;

	vec3 f = fract(uv);
	f = f*f*(3.f - 2.f*f);

	vec4 v = vec4{
		uv0.x + uv0.y + uv0.z, uv1.x + uv0.y + uv0.z,
		uv0.x + uv1.y + uv0.z, uv1.x + uv1.y + uv0.z};

	vec4 r = fract(sin(v*1e-1f) * 1e3f);
	float r0 = mix(mix(r.x, r.y, f.x), mix(r.z, r.w, f.x), f.y);

	r = fract(sin((v + uv1.z - uv0.z)*1e-1f) * 1e3f);
	float r1 = mix(mix(r.x, r.y, f.x), mix(r.z, r.w, f.x), f.y);

	return mix(r0, r1, f.z)*2.f - 1.f;
}

vec3 main_image(vec2 const & uv)  // vrati farbu pixelu
{
	vec2 p = uv - .5f;

	float color = 3.f - 3.f*length(2.f*p);

	vec3 coord = vec3(atan(p.x, p.y) / (2*M_PI) + .5f, length(p)*.4, .5);

	for (int i = 1; i <= 7; ++i)  // medzi 5 a 7 uz neviem rozpoznt rozdiel (600x600)
	{
		float power = pow(2.f, (float)i);
		color += (1.5f / power) * snoise(coord, power*16);
	}

	return vec3{color, pow(max(color, 0.f), 2.f)*.4f, pow(max(color, 0.f), 3.f)*.15f};
}

uint8_t to_byte(float f)
{
	long v = f * 255;
	if (v > 255)
		return 255;
	else if (v < 0)
		return 0;
	else
		return (uint8_t)v;
}

rgb8_pixel_t pixel_cast(vec3 const & v)
{
	return rgb8_pixel_t{to_byte(v.x), to_byte(v.y), to_byte(v.z)};
}


int main(int argc, char * argv[])
{
	int w = 600, h = 600;
	rgb8_image_t im{w, h};

	rgb8_view_t v = view(im);
	for (int y = 0; y < h; ++y)
	{
		for (int x = 0; x < w; ++x)
		{
			vec2 uv = vec2{x,y} / vec2{w,h};
			vec3 p = main_image(uv);
			v(x,y) = pixel_cast(p);
		}
	}

	png_write_view("out_image.png", flipped_up_down_view(v));

	cout << "done.\n";
	return 0;
}

/* https://www.shadertoy.com/view/lsf3RH

float snoise(vec3 uv, float res)
{
	const vec3 s = vec3(1e0, 1e2, 1e3);

	uv *= res;

	vec3 uv0 = floor(mod(uv, res))*s;
	vec3 uv1 = floor(mod(uv+vec3(1.), res))*s;

	vec3 f = fract(uv); f = f*f*(3.0-2.0*f);

	vec4 v = vec4(uv0.x+uv0.y+uv0.z, uv1.x+uv0.y+uv0.z,
					  uv0.x+uv1.y+uv0.z, uv1.x+uv1.y+uv0.z);

	vec4 r = fract(sin(v*1e-1)*1e3);
	float r0 = mix(mix(r.x, r.y, f.x), mix(r.z, r.w, f.x), f.y);

	r = fract(sin((v + uv1.z - uv0.z)*1e-1)*1e3);
	float r1 = mix(mix(r.x, r.y, f.x), mix(r.z, r.w, f.x), f.y);

	return mix(r0, r1, f.z)*2.-1.;
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
	vec2 p = -.5 + fragCoord.xy / iResolution.xy;
	p.x *= iResolution.x/iResolution.y;

	float color = 3.0 - (3.*length(2.*p));

	vec3 coord = vec3(atan(p.x,p.y)/6.2832+.5, length(p)*.4, .5);

	for(int i = 1; i <= 7; i++)
	{
		float power = pow(2.0, float(i));
		color += (1.5 / power) * snoise(coord + vec3(0.,-iGlobalTime*.05, iGlobalTime*.01), power*16.);
	}
	fragColor = vec4( color, pow(max(color,0.),2.)*0.4, pow(max(color,0.),3.)*0.15 , 1.0);
}
*/
