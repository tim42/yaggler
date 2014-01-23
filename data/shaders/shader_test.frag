

// test fragment shader
// to see if yaggler works ;)

// Specify which version of GLSL we are using.

#version 130

uniform float iGlobalTime;
uniform vec2 iResolution;
uniform sampler2D iTexture;
uniform sampler2D iFGTexture;

out vec4 output_color;
in vec4 position;

// defines
#define RM_STEP         55.
#define RM_INC          0.00435

// consts
const float init_time = 5.;
const vec3 origin = vec3(0.,1.11,0.);

float length8(in vec3 v)
{
  return pow(pow(v.x, 8.0) + pow(v.y, 8.0) + pow(v.z, 8.0), 1. / 8.);
}
float length8(in vec2 v)
{
  return pow(pow(v.x, 8.0) + pow(v.y, 8.0), 1. / 8.);
}

float d(vec3 p, float r)
{
  vec3 c = vec3(30. /** (cos(p.z / 50.) + 1.0)*/, 30. /** (cos(p.x / 60. + sin(iGlobalTime)) + 1.0)*/, 30.);
 p = p + vec3(-20. * cos(iGlobalTime / 5.), -3.0 + 20. * sin(iGlobalTime / 5.) , -6. + iGlobalTime * 13.);
  p = p + vec3(-5, -6, -6. + iGlobalTime * 3.);
  p =  mod(p, c) - 0.5 * c;

  float d1 = length(max(abs(p /*+ vec3(0, -0.7, 0.5)*/) - vec3(r, r, r), 0.00));
  vec3 q = abs(p);
  float d2 = max(q.z-r*1.1,max(q.x+q.y*0.57735,q.y*1.1547)-r*1.1);
  
  float d3 = length(p ) - r;

//    return d2;
   return max(d1, -d2)/*+ 0.01*/;
  return mix(d2, d1, (sin(iGlobalTime / 8.)+1.) / 2.);
}

float map(vec3 p)
{
  return d(p, 2.);
}

vec3 normal(in vec3 pos)
{
  vec2 eps = vec2(0.001, 0.0);
  vec3 nor = vec3(
      map(pos + eps.xyy) - map(pos - eps.xyy),
      map(pos + eps.yxy) - map(pos - eps.yxy),
      map(pos + eps.yyx) - map(pos - eps.yyx));
  return normalize(nor);
}

vec3 raymarch(in vec3 from, in vec3 dir, in vec3 ocolor, const float max_it)
{
  vec3 color = vec3(0, 0, 0);

  float dstacc = 0.;
  float rst;
  float td = 0.;
  float c_fact = 1.;
  for (float it = 0.; it < max_it && c_fact > 0.009; ++it)
  {
    rst = map(from + dir * dstacc);
    dstacc += (rst);

//     dir = refract(dir, vec3(0, 0, -1), 1.00101);

    if (rst < 0.001 && td != 1.)
    {
//       color += vec3(0.19, 0.9, 0.001 + 1./length(from)/250. ) * (c_fact) * (1./length(from) / 50.);
      color += vec3(abs(vec3(150., 125. + 25.*cos(iGlobalTime), 125. + 25.*sin(iGlobalTime)) - from.xyz) / 569.) * (c_fact);
      c_fact *= 0.22;
      td = 1.;
      return color;
//        color = (normal(from + dir * dstacc) + vec3(1., 1., 1.)) / 2.;break;
break;
//       vec3 tdr = reflect(dir, normal(from + dir * dstacc));
       vec3 tdr = refract(dir, normal(from + dir * dstacc), 1.000);
//       dstacc += 0.01;
      from = from + dir * dstacc;
      dir = tdr;
//       break;
//        color += vec3(0.00050, 0.00001, 0.00001) * 1.*(0.01-rst);
      //color *= 1.035;
      //dstacc += (0.5) / 1.10;
    }
    else
    {
      if (rst < 0.001 && td == 1.)
      {
//              color += vec3((vec3(150, 125 + 25.*cos(iGlobalTime), 125 + 25.*sin(iGlobalTime)) - from.xyz) / 569.) * (c_fact);

//       color += vec3(1., 0., 0.);
//         dstacc += -2.*rst;
//       dstacc += RM_INC;
//         color += vec3(0.01, 0.0001, 0.001) * c_fact;
      }
      else
      {
        color += (1.-td) * (0.0009 + ((1.0 - (clamp(rst + 1.0, 1.0, 2.0) - 1.))) / (100. + 70. * cos(iGlobalTime))) * c_fact; // depth + glow
        td = 0.;
      }
    }
  }
  return color + ocolor;
}

vec4 color_init(in vec2 uv)
{
  float t = clamp(iGlobalTime, 0., init_time) / init_time;
  float invt = 1.0 - t;

//   float ct = 

  //return vec4(invt*uv.xy + vec2(t, t), invt + t, 1. - clamp(t - .9, 0., 0.1) * 10.);
  return vec4(0., 0., 0., 1. - t);
}

vec2 ripple(in vec2 uv)
{
   uv.x = uv.x+sin(uv.x)/5.;
   uv.y = uv.y+sin(uv.y)/5.;
   return uv;
}


void main()
{
  vec2 FragCoord = (position.xy / 2 + 0.5) * iResolution;
  vec4 tcolor = texture2D(iTexture, vec2(FragCoord.xy * 5 / iResolution.xy));
  vec4 fgcolor = texture2D(iFGTexture, vec2(((FragCoord.xy / (iResolution / 250)) + vec2(sin(iGlobalTime / 1.5) * 250 + 250, 0) + vec2(0, cos(iGlobalTime) * 250 + 250)) / iResolution.xy));

  tcolor.rgb *= tcolor.a; // blend alpha
  fgcolor.rgb *= fgcolor.a; // blend alpha
  fgcolor.a = 1;

  vec2 uv = FragCoord.xy / iResolution.xy * 2.0 - 1.0;

  uv.y *= iResolution.y / iResolution.x;
  vec3 from = origin;

  //vec3 color = raymarch(from, dir);
  uv = ripple(uv);
  vec3 dir = normalize(vec3(uv * 0.8 , 1.0));
  vec4 color = color_init(uv * iGlobalTime);

  color = color * color.w + vec4(raymarch(from, dir, tcolor.rgb, RM_STEP), 1.0) * (1. - color.w);

  // set the color.
  output_color =  (fgcolor) / vec4(color.xyz, 1.0) ;
//   output_color = normalize(output_color) * 2;
}


// "GENERATORS REDUX" by Kali 

// Reworked by eiffie to run faster and under ANGLE:
//
// -Made the sphere raytraced 
// -Soft AO shadows
// -Various great optimizations
// 
// Thanks eiffie!!!


// Original description:
// Same fractal as "Ancient Temple" + rotations, improved shading 
// (better coloring, AO and  shadows), some lighting effects, and a path for the camera  
// following a liquid metal ball. 


//#define ENABLE_HARD_SHADOWS // turn off to enable faster AO soft shadows 
//#define ENABLE_VIBRATION
//#define ENABLE_POSTPROCESS // Works better on window view rather than full screen

#if 0
#define RAY_STEPS 90
#define SHADOW_STEPS 35
#define LIGHT_COLOR vec3(.85,.9,1.)
#define AMBIENT_COLOR vec3(.75,0.9,1.0)
#define FLOOR_COLOR vec3(1.,.7,.2)
#define ENERGY_COLOR vec3(0.5,1.0,.5)
#define BRIGHTNESS 1.0
#define GAMMA 1.9
#define SATURATION 0.650


#define detail .00004
#define t iGlobalTime*.25



vec3 lightdir=normalize(vec3(0.5,-0.3,-1.));
vec3 ambdir=normalize(vec3(0.,0.,1.));
const vec3 origin=vec3(0.,3.11,0.);
vec3 energy=vec3(0.1);
#ifdef ENABLE_VIBRATION
float vibration=sin(iGlobalTime*60.)*.0013;
#else
float vibration=0.;
#endif
float det=0.0;
vec3 pth1;


mat2 rot(float a) {
        return mat2(cos(a),sin(a),-sin(a),cos(a));      
}


vec3 path(float ti) {
return vec3(sin(ti),-0.71-sin(ti*1.0632)*.3,cos(ti*.5))*.5;
}

float Sphere(vec3 p, vec3 rd, float r){//A RAY TRACED SPHERE
        float b = dot( -p, rd );
        float inner = b * b - dot( p, p ) + r * r;
        if( inner < 0.0 ) return -1.0;
        return b - sqrt( inner );
}

vec2 de(vec3 pos) {
        float hid=0.;
        vec3 tpos=pos;
        tpos.xz=abs(.5-mod(tpos.xz,1.));
        vec4 p=vec4(tpos,1.);
        float y=max(0.,1.0-abs(pos.y-3.0))/1.0;
        for (int i=0; i<13; i++)
        {
          //LOWERED THE ITERS
                p.xyz = abs(p.xyz)-vec3(-0.05,2.,-0.01);
                p=p*(2.0+vibration*y)/clamp(dot(p.xyz,p.xyz),.1,1.10)-vec4(5.0,0.0,-0.90,-3.1000);
                p.yz*=mat2(-0.516,-0.91,0.91,-0.516);
        }
        float fl=pos.y-3.013;
        //float fr=(length(max(abs(p.xyz)-vec3(0.1,5.0,0.1),vec3(0.0)))-0.05)/p.w;//RETURN A RRECT
        float fr=length(p.xyz)/p.w;
        float d=min(fl,fr);
        d=min(d,-pos.y+3.95);
        //if (abs(d-fl)<.001) hid=1.;
        return vec2(fr,hid);
}


vec3 normal(vec3 p) {
        vec3 e = vec3(0.0,det,0.0);
        
        return normalize(vec3(
                        de(p+e.yxx).x-de(p-e.yxx).x,
                        de(p+e.xyx).x-de(p-e.xyx).x,
                        de(p+e.xxy).x-de(p-e.xxy).x
                        )
                );      
}

float shadow(vec3 pos, vec3 sdir) {//THIS ONLY RUNS WHEN WITH HARD SHADOWS
        float sh=1.0;
        float totdist =2.0*det;
        float dist=10.;
        float t1=Sphere((pos-.005*sdir)-pth1,-sdir,0.015);
        if (t1>0. && t1<.5) {
                vec3 sphglowNorm=normalize(pos-t1*sdir-pth1);
                sh=1.-pow(max(.0,dot(sphglowNorm,sdir))*1.2,3.);
        } 
                for (int steps=0; steps<SHADOW_STEPS; steps++) {
                        if (totdist<.6 && dist>detail) {
                                vec3 p = pos - totdist * sdir;
                                dist = de(p).x;
                                sh = min( sh, max(50.*dist/totdist,0.0) );
                                totdist += max(.01,dist);
                        }
                }
        
    return clamp(sh,0.1,1.0);
}


float calcAO( const vec3 pos, const vec3 nor ) {
        float aodet=detail*40.;
        float totao = 0.0;
    float sca = 14.0;
    for( int aoi=0; aoi<5; aoi++ ) {
        float hr = aodet*float(aoi*aoi);
        vec3 aopos =  nor * hr + pos;
        float dd = de( aopos ).x;
        totao += -(dd-hr)*sca;
        sca *= 0.7;
    }
    return clamp( 1.0 - 5.0*totao, 0., 1.0 );
}

float texture(vec3 p) {
        p=abs(.5-fract(p*10.));
        vec3 c=vec3(3.);
        float es, l=es=0.;
        for (int i = 0; i < 10; i++) { 
                        p = abs(p + c) - abs(p - c) - p; 
                        p/= clamp(dot(p, p), .0, 1.);
                        p = p* -1.5 + c;
                        if ( mod(float(i), 2.) < 1. ) { 
                                float pl = l;
                                l = length(p);
                                es+= exp(-1. / abs(l - pl));
                        }
        }
        return es;
}

vec3 light(in vec3 p, in vec3 dir, in vec3 n, in float hid) {//PASSING IN THE NORMAL
        #ifdef ENABLE_HARD_SHADOWS
                float sh=shadow(p, lightdir);
        #else
                float sh=calcAO(p,-2.5*lightdir);//USING AO TO MAKE VERY SOFT SHADOWS
        #endif
        float ao=calcAO(p,n);
        float diff=max(0.,dot(lightdir,-n))*sh;
        float y=3.35-p.y;
        vec3 amb=max(.5,dot(dir,-n))*.5*AMBIENT_COLOR;
        if (hid<.5) {
                amb+=max(0.2,dot(vec3(0.,1.,0.),-n))*FLOOR_COLOR*pow(max(0.,.2-abs(3.-p.y))/.2,1.5)*2.;
                amb+=energy*pow(max(0.,.4-abs(y))/.4,2.)*max(0.2,dot(vec3(0.,-sign(y),0.),-n))*2.;
        }
        vec3 r = reflect(lightdir,n);
        float spec=pow(max(0.,dot(dir,-r))*sh,10.);
        vec3 col;
        float energysource=pow(max(0.,.04-abs(y))/.04,4.)*2.;
        if (hid>1.5) {col=vec3(1.); spec=spec*spec;}
        else{
                float k=texture(p)*.23+.2; 
                k=min(k,1.5-energysource);
                col=mix(vec3(k,k*k,k*k*k),vec3(k),.3);
                if (abs(hid-1.)<.001) col*=FLOOR_COLOR*1.3;
        }
        col=col*(amb+diff*LIGHT_COLOR)+spec*LIGHT_COLOR;        
        if (hid<.5) { 
                col=max(col,energy*2.*energysource);
        }
        col*=min(1.,ao+length(energy)*.5*max(0.,.1-abs(y))/.1);
        return col;
}

vec3 raymarch(in vec3 from, in vec3 dir) 

{
        float ey=mod(t*.5,1.);
        float glow,eglow,ref,sphdist,totdist=glow=eglow=ref=sphdist=0.;
        vec2 d=vec2(1.,0.);
        vec3 p, col=vec3(0.);
        vec3 origdir=dir,origfrom=from,sphNorm;
        
        //FAKING THE SQUISHY BALL BY MOVING A RAY TRACED BALL
      /*  vec3 wob=cos(dir*500.0*length(from-pth1)+(from-pth1)*250.+iGlobalTime*10.)*0.0005;
        float t1=Sphere(from-pth1+wob,dir,0.015);
        float tg=Sphere(from-pth1+wob,dir,0.02);
        if(t1>0.){
                ref=1.0;from+=t1*dir;sphdist=t1;
                sphNorm=normalize(from-pth1+wob);
                dir=reflect(dir,sphNorm);
        } 
        else if (tg>0.) { 
                vec3 sphglowNorm=normalize(from+tg*dir-pth1+wob);
                glow+=pow(max(0.,dot(sphglowNorm,-dir)),5.);
        };
        */
        for (int i=0; i<RAY_STEPS; i++) {
                if (d.x>det && totdist<5.30) {
                        p=from+totdist*dir;
                        d=de(p);
                        det=detail*(1.+totdist*60.)*(1.+ref*5.);
                        totdist+=d.x; 
                        energy=ENERGY_COLOR*(1.5+sin(iGlobalTime*20.+p.z*10.))*.25;
                        if(d.x<0.015)glow+=max(0.,.015-d.x)*exp(-totdist);
                        if (d.y<.5 && d.x<0.3){//ONLY DOING THE GLOW WHEN IT IS CLOSE ENOUGH
                                float glw=min(abs(3.35-p.y-ey),abs(3.35-p.y+ey));//2 glows at once
                                eglow+=max(0.,.03-d.x)/.03*
                                (pow(max(0.,.05-glw)/.05,5.)
                                +pow(max(0.,.15-abs(3.35-p.y))/.15,8.))*1.5;
                        }
                }
        }
        float l=pow(max(0.,dot(normalize(-dir.xz),normalize(lightdir.xz))),2.);
        l*=max(0.2,dot(-dir,lightdir));
        vec3 backg=.5*(1.2-l)+LIGHT_COLOR*l*.7;
        backg*=AMBIENT_COLOR;
        if (d.x<=det) {
                vec3 norm=normal(p-abs(d.x-det)*dir);//DO THE NORMAL CALC OUTSIDE OF LIGHTING (since we already have the sphere normal)
                col=light(p-abs(d.x-det)*dir, dir, norm, d.y)*exp(-.9*totdist*totdist); 
                col = mix(col, backg, 1.0-exp(-1.*pow(totdist,1.5)));
        } else { 
                col=backg;
        }
        vec3 lglow=LIGHT_COLOR*pow(l,30.)*.5;
        col+=glow*(backg+lglow)*1.3;
        col+=pow(eglow,2.)*energy*.015;
        col+=lglow*min(1.,totdist*totdist*.3);
        if (ref>0.5) {
                vec3 sphlight=light(origfrom+sphdist*origdir,origdir,sphNorm,2.);
                col=mix(col*.3+sphlight*.7,backg,1.0-exp(-1.*pow(sphdist,1.5)));
        }
        return col; 
}

vec3 move(inout mat2 rotview1,inout mat2 rotview2) {
        vec3 go=path(t);
        vec3 adv=path(t+.7);
        vec3 advec=normalize(adv-go);
        float an=atan(advec.x,advec.z);
        rotview1=mat2(cos(an),sin(an),-sin(an),cos(an));
                  an=advec.y*1.7;
        rotview2=mat2(cos(an),sin(an),-sin(an),cos(an));
        return go;
}


void main(void)
{
        gl_FragColor = vec4(1., 0., 0., 0.);
return coincoinSfasf;
        pth1 = path(t+.3)+origin+vec3(0.,.01,0.);
        vec2 uv = gl_FragCoord.xy / iResolution.xy*2.-1.;
        vec2 uv2=uv;
#ifdef ENABLE_POSTPROCESS
        uv*=1.+pow(length(uv2*uv2*uv2*uv2),4.)*.07;
#endif
        uv.y*=iResolution.y/iResolution.x;
        
        mat2 rotview1, rotview2;
        vec3 from=origin+move(rotview1,rotview2);
        vec3 dir=normalize(vec3(uv*.8,1.));
        
        dir.yz*=rotview2;
        dir.xz*=rotview1;
        vec3 color=raymarch(from,dir); 
        color=clamp(color,vec3(.0),vec3(1.));
        color=pow(color,vec3(GAMMA))*BRIGHTNESS;
        color=mix(vec3(length(color)),color,SATURATION);
#ifdef ENABLE_POSTPROCESS
        
        color*=1.-pow(length(uv2*uv2*uv2*uv2)*1.1,6.);
        uv2.y *= iResolution.y / 360.0;
        color.r*=(.5+abs(.5-mod(uv2.y     ,.021)/.021)*.5)*1.5;
        color.g*=(.5+abs(.5-mod(uv2.y+.007,.021)/.021)*.5)*1.5;
        color.b*=(.5+abs(.5-mod(uv2.y+.014,.021)/.021)*.5)*1.5;
       
#endif
        gl_FragColor = vec4(color,1.);
}
#endif