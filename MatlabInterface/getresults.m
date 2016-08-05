function results=getresults(f, p)
%GETRESULTS Reads binary file.
%
%   results = GETRESULTS( f, p) Reads the binary file
%   with filename f and population p.
%
% Syntax:  results = getresults( f, p )
%
% Inputs:
%    f - Filename of the binary file.
%    p - Population size.
%
% Outputs:
%    results - Data of the binary file.
%
% Example:
%    results  = getresults( 'outdata.bin', 1024 )
%    This example reads the data of a binary file with filename
%    'outdata.bin'. The data are from population size 1024.
%
%
% Other m-files required: none
% Subfunctions: none
% MAT-files required: none
%
% See also: nan
%
% $Author: Eleftherios Avramidis $
% $Email: el.avramidis@gmail.com $
% $Date: 2015/05/13 $
% $Version: 1.0 $
% Copyright: MIT License

fid = fopen(f);
currData = fread(fid, 'double');
fclose(fid);
results = vec2mat(currData,p);
end
