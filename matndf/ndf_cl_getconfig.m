%   Copyright (C) 2010 Michele Tavella <tavella.michele@gmail.com>
%
%   This file is part of matndf
%
%   The libndf library is free software: you can redistribute it and/or
%   modify it under the terms of the version 3 of the GNU General Public
%   License as published by the Free Software Foundation.
%
%   This program is distributed in the hope that it will be useful,
%   but WITHOUT ANY WARRANTY; without even the implied warranty of
%   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
%   GNU General Public License for more details.
%
%   You should have received a copy of the GNU General Public License
%   along with this program.  If not, see <http://www.gnu.org/licenses/>.
%
%   function cfg = ndf_cl_getconfig(name);
function cfg = ndf_cl_getconfig(cl, name, pn, aD, aC);

	if(nargin < 3); pn = ''; end
if(nargin < 4); aD = ''; end
if(nargin < 5); aC = ''; end

% Retrieve from nameserver:
% - block		the XML block (i.e. mi)
% - taskset		the XML taskset (i.e. mi_rhlh)
% - xml			the XML filename
% - path		the working directory
cfg = {};
cfg.ns.block   = cl_retrieve(cl, [name '::block']);
cfg.ns.taskset = cl_retrieve(cl, [name '::taskset']);
cfg.ns.xml     = cl_retrieve(cl, [name '::xml']);
cfg.ns.path    = cl_retrieve(cl, [name '::path']);

cfg.config = ccfg_new();
if(ccfg_importfile(cfg.config, cfg.ns.xml) == 0)
	disp('[ndf_cl_config] Cannot load XML file, killing matlab');
	exit;
end

% Use ccfg_* to load the XML and retrieve the taskset. 
cfg.messageC = icmessage_new();
cfg.taskset = ...
	ccfg_onlinem(cfg.config, cfg.ns.block, cfg.ns.taskset, cfg.messageC);
if(cfg.taskset == 0)	
	disp('[ndf_cl_config] Cannot retrieve taskset, killing matlab');
	exit;
end

% The taskset contains informations about:
% - the classifier
% - the NDF configuration (pipes etc)
[cfg.classifier.name, cfg.classifier.desc, cfg.classifier.file] = ...
	ccfgtaskset_getclassifier(cfg.taskset);
[cfg.ndf.function, cfg.ndf.pn, cfg.ndf.id, cfg.ndf.ic] = ...
	ccfgtaskset_getndf(cfg.taskset);


% Override NDF option from XML if needed
if(isempty(aC) == false); 
	disp('[ndf_cl_config] Overriding iC address');
	cfg.ndf.ic = aC;
end
if(isempty(aD) == false); 
	disp('[ndf_cl_config] Overriding iD address');
	cfg.ndf.id = aD;
end
if(isempty(pn) == false); 
	disp('[ndf_cl_config] Overriding pipename');
	cfg.ndf.pn = pn;
end

fprintf(1, '[ndf_cl_config] Nameserver configuration:\n');
fprintf(1, '  Block:    %s\n', cfg.ns.block);
fprintf(1, '  Taskset:  %s\n', cfg.ns.taskset);
fprintf(1, '  XML:      %s\n', cfg.ns.xml);
fprintf(1, '  Path:     %s\n', cfg.ns.path);
fprintf(1, '[ndf_cl_config] NDF configuration:\n');
fprintf(1, '  Function: %s\n', cfg.ndf.function);
fprintf(1, '  Pipename: %s\n', cfg.ndf.pn);
fprintf(1, '  TOBI iD:  %s\n', cfg.ndf.id);
fprintf(1, '  TOBI iC:  %s\n', cfg.ndf.ic);
fprintf(1, '[ndf_cl_config] Classifier configuration:\n');
fprintf(1, '  Name      %s\n', cfg.classifier.name);
fprintf(1, '  Desc.:    %s\n', cfg.classifier.desc);
fprintf(1, '  Filename: %s\n', cfg.classifier.file);