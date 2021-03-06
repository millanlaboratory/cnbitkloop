% Copyright (C) 2009-2011  EPFL (Ecole Polytechnique Fédérale de Lausanne)
% Michele Tavella <michele.tavella@epfl.ch>
%
% This program is free software: you can redistribute it and/or modify
% it under the terms of the GNU General Public License as published by
% the Free Software Foundation, either version 3 of the License, or
% (at your option) any later version.
%
% This program is distributed in the hope that it will be useful,
% but WITHOUT ANY WARRANTY; without even the implied warranty of
% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
% GNU General Public License for more details.
%
% You should have received a copy of the GNU General Public License
% along with this program.  If not, see <http://www.gnu.org/licenses/>.
function loop = ndf_loopconfig(loop, name) %, pn, aD, aC);

% Retrieve from nameserver:
% - block		the XML block (i.e. mi)
% - taskset		the XML taskset (i.e. mi_rhlh)
% - xml			the XML filename
% - path		the working directory
loop.cfg = {};
loop.cfg.ns.modality = cl_retrieveconfig(loop.cl, name, 'modality');
loop.cfg.ns.block    = cl_retrieveconfig(loop.cl, name, 'block');
loop.cfg.ns.taskset  = cl_retrieveconfig(loop.cl, name, 'taskset');
loop.cfg.ns.xml      = cl_retrieveconfig(loop.cl, name, 'xml');
loop.cfg.ns.path     = cl_retrieveconfig(loop.cl, name, 'path');

loop.cfg.config = ccfg_new();
if(ccfg_importfile(loop.cfg.config, loop.cfg.ns.xml) == 0)
	disp('[ndf_loopconfig] Cannot load XML file');
	loop.cfg.config = ccfg_delete(loop.cfg.config);
	return;
end

% Use ccfg_* to load the XML and retrieve the taskset. 
loop.cfg.taskset = ccfg_online(loop.cfg.config, loop.cfg.ns.block, loop.cfg.ns.taskset, ...
	loop.mC, loop.mDo);
if(loop.cfg.taskset == 0)	
	disp('[ndf_loopconfig] Cannot retrieve taskset');
	loop.cfg.config = ccfg_delete(loop.cfg.config);
	return;
end


% The taskset contains informations about:
% - the classifier
% - the NDF configuration (pipes etc)
[loop.cfg.classifier.id, loop.cfg.classifier.desc, loop.cfg.classifier.file] = ...
	ccfgtaskset_getclassifier(loop.cfg.taskset);
[loop.cfg.ndf.exec, loop.cfg.ndf.pipe, loop.cfg.ndf.id, loop.cfg.ndf.ic] = ...
	ccfgtaskset_getndf(loop.cfg.taskset);

fprintf(1, '[ndf_loopconfig] Nameserver configuration:\n');
fprintf(1, '  Modality: %s\n', loop.cfg.ns.modality);
fprintf(1, '  Block:    %s\n', loop.cfg.ns.block);
fprintf(1, '  Taskset:  %s\n', loop.cfg.ns.taskset);
fprintf(1, '  XML:      %s\n', loop.cfg.ns.xml);
fprintf(1, '  Path:     %s\n', loop.cfg.ns.path);
fprintf(1, '[ndf_loopconfig] NDF configuration:\n');
fprintf(1, '  Exec:     %s\n', loop.cfg.ndf.exec);
fprintf(1, '  Pipe:     %s\n', loop.cfg.ndf.pipe);
fprintf(1, '  TOBI iD:  %s\n', loop.cfg.ndf.id);
fprintf(1, '  TOBI iC:  %s\n', loop.cfg.ndf.ic);
fprintf(1, '[ndf_loopconfig] Classifier configuration:\n');
fprintf(1, '  Id:       %s\n', loop.cfg.classifier.id);
fprintf(1, '  Desc.:    %s\n', loop.cfg.classifier.desc);
fprintf(1, '  Filename: %s\n', loop.cfg.classifier.file);

fprintf(1, '[ndf_loopconfig] TOBI interfaces status:\n');
if(tid_attach(loop.iD, loop.cfg.ndf.id) == true)
	fprintf(1, '  TOBI iD:  attached to %s\n', loop.cfg.ndf.id);
else
	fprintf(1, '  TOBI iD:  not attached\n');
end

if(tic_attach(loop.iC, loop.cfg.ndf.ic) == true)
	fprintf(1, '  TOBI iC:  attached to %s\n', loop.cfg.ndf.ic);
else
	fprintf(1, '  TOBI iC:  not attached\n');
end
