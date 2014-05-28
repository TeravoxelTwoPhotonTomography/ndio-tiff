function savestack(filename, a)


t=Tiff(filename,'w');

if(length(size(a))<3)
  push(a);
else
  for i=1:size(a,3)
    push(a(:,:,i));
  end
end

t.close();

  function push(im)
    t.setTag(struct(...,
      'ImageLength',size(a,1),...
      'ImageWidth',size(a,2),...
      'Photometric',Tiff.Photometric.MinIsBlack,...
      'BitsPerSample',bpp(a),...
      'SampleFormat',fmt(a),...
      'PlanarConfiguration',Tiff.PlanarConfiguration.Chunky,...
      'SamplesPerPixel',1,...
      'Orientation',Tiff.Orientation.TopLeft,...
      'Compression',Tiff.Compression.None...
      ));
    t.write(im);
    t.writeDirectory();
  end

  function v=fmt(a)
    u=Tiff.SampleFormat.UInt;
    i=Tiff.SampleFormat.Int;
    f=Tiff.SampleFormat.IEEEFP;
    v=getfield(struct(...,
      'logical',u,...
      'uint8',u,...
      'uint16',u,...
      'uint32',u,...
      'uint64',u,...
      'int8',i,...
      'int16',i,...
      'int32',i,...
      'int64',i,...
      'single',f,...
      'double',f),...
      class(a));
  end

  function v=bpp(a)
    switch class(a)
      case 'logical'
        v=1;
      case 'uint8'
        v=8;
      case 'int8'
        v=8;
      case 'uint16'
        v=16;
      case 'int16'
        v=16;
      case 'uint32'
        v=32;
      case 'int32'
        v=32;
      case 'uint64'
        v=64;
      case 'int64'
        v=64;
      case 'single'
        v=32;
      case 'double'
        v=64;
    end
  end

end