function [tout,yout]  = odecl( platform, device, kernel, initx, params, solver, orbits, nequat, nparams, nnoi, dt, tspan, ksteps, localgroupsize )
%ODECL Interface for using the odecl solvers in MATLAB.
%
% Syntax:  [tout,yout] = ODECL( platform, device, kernel, initx, params, solver, orbits, nequat, nparams, nnoi, dt, tspan, ksteps )
%
% Inputs:
%   platform    - OpenCL platform number
%   device      - OpenCL device number of selected platform
%   kernel      - ODE system kernel filename
%   initx       - Initial conditions for each orbit for the ODE system
%   params      - Parameters sets for the ODE system for all orbits
%   solver      - ODE solver
%   orbits      - Number of orbits to be integrated
%   nequat      - Number of equations of the ODE system
%   nparams     - Number of parameter of the ODE system
%   nnoi        - Number of noise processes
%   dt          - ODE solver time step size
%   tspan       - Integration time span
%   ksteps      - Number of ODE solvers executed in each OpenCL kernel call
%
%
% Outputs:
%   yout - Time output
%   yout - Model output
%
% Example:
%   [tout,yout]  = odecl( 3 0 broomhead.cl x_y0.txt x_params.txt 2 512 6 6 1e-5 6 40)
%
% Other m-files required: none
% Subfunctions: none
% MAT-files required: none
% Other dependencies: odecl executable
%
% See also: nan
%
% $Author: Eleftherios Avramidis $
% $Email: el.avramidis@gmail.com $
% $Date: 2016/07/25 $
% $Version: 0.1 $
% Copyright: MIT License
% tic

delete('t.bin');
delete('odecloutput.bin');
delete('odecllog.txt');
delete('x_params.bin');
delete('x_t0.bin');
delete('x_y0.bin');

% Write parameters and initial conditions to files

% dlmwrite('x_y0.txt', initx, 'delimiter', '\t','precision',10);
fileID = fopen('x_y0.bin','w');
fwrite(fileID,initx','double');
fclose(fileID);

t0=0;
x_t0=repmat(t0, orbits, 1);

% dlmwrite('x_t0.txt', x_t0, 'delimiter', '\t','precision',10);
fileID = fopen('x_t0.bin','w');
fwrite(fileID,x_t0','double');
fclose(fileID);

% dlmwrite('x_params.txt', params, 'delimiter', '\t','precision',10);
fileID = fopen('x_params.bin','w');
fwrite(fileID,params','double');
fclose(fileID);

switch solver
    case 'e'
        odesolver=0;
    otherwise
        odesolver=0;
end

% Run the opencl program
[s w] = dos(['SampleModel.exe ' num2str(platform) ' ' num2str(device) ... 
    ' ' kernel ' ' 'x_y0.bin' ' '  'x_params.bin' ' ' num2str(odesolver) ...
    ' ' num2str(orbits) ' ' num2str(nequat) ' ' num2str(nparams) ' ' num2str(nnoi) ...
    ' ' num2str(dt) ' ' num2str(tspan) ' ' num2str(ksteps) ' ' num2str(localgroupsize)], '-echo');
if s % then failed
    disp(' Call to SDECL failed')
    return
end

% get the results
yout=getresults('odecloutput.bin', orbits);
tout=0:dt*ksteps:(length(yout)-1)*dt*ksteps;

