//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package source_sinc.simulations;

import source_sinc.Source;
import source_sinc.Sink;

//
// Sample network, consisting of a source and a sink.
//
network SourceSink
{
    @display("bgb=562,75");
    submodules:
        source: Source;
        sink: Sink {
            @display("p=320,21");
        }
    connections:
        source.out --> sink.in;
}
