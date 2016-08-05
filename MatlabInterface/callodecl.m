% 05/08/2016
% Eleftherios Avramidis
% ODECL Caller

close all;
clear all;

pop=8;

% Broomhead et al. (2000)
y0=[0 0 0 0 0 2];
t0=0;

x_t0=repmat(t0, pop, 1);
x_y0=repmat(y0, pop, 1);

% Uniform parameters
x0=[120 1.5 0.0045 0.05 600 9];
x_params=repmat(x0,pop,1);

% Random parameters
% a = [1,        0.0001,    0.001,       0,         1,      0.00001];
% b = [1000,     60,        0.1,         6.0,        1000,   60 ];
% x_params = repmat(a,pop,1) + repmat((b-a),pop,1).*rand(pop,6);

platform = 3; 
device = 0;
kernel = 'broomhead.cl';
initx = x_y0;
params = x_params;
solver = 'im';
orbits = pop;
nequat = 6;
nparams = 6;
dt = 1e-5;
tspan = 6;
ksteps = 40;

% run simulations
delete('t.bin');
delete('odecloutput.bin');
[tout,yout]=odecl(platform, device, kernel, initx, params, solver, orbits, nequat, nparams, dt, tspan, ksteps );

plot(yout(:,1),'r');
hold on
% load('jerkexpfloat.mat')
% plot(jerkexpfloat,'b--');

load('xpp.mat')
plot(Y(:,1),'b--');