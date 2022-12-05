package cyana.ast.runtime;

import java.io.IOException;
import java.io.InputStream;

/**
 * 类似BufferedInpustream功能的流.
 *
 * @author tian wei jun
 */
public class ByteBufferedInputStream {
  private final int standardBufferCapacity = 1024;
  public int nextReadIndex = 0;
  private int eof = -1;
  private int nextPos = 0;
  private int count = 0;
  private int mark = eof;
  private byte[] buffer;
  private InputStream byteInputStream;

  /**
   * 带参构造.
   *
   * @param byteInputStream 所识别的文本输入流
   */
  public ByteBufferedInputStream(InputStream byteInputStream) {
    this.byteInputStream = byteInputStream;
    init();
  }

  /** 初始化. */
  public void init() {
    buffer = new byte[standardBufferCapacity];
  }

  /**
   * 读取一个字符.
   *
   * @return 所读取的字符，若没有文本了，返回-1
   */
  public int read() {
    int read = eof;
    if (nextPos < count) {
      read = buffer[nextPos++];
      ++nextReadIndex;
    } else {
      if (fill()) {
        read = read();
      }
    }
    return read;
  }

  /**
   * 只有当读完缓冲后才能调用，其他时候不能调用，只能在read方法中调用.
   *
   * @return
   */
  private boolean fill() {
    if (mark == eof) { // 重新填入
      return fillByNoMark();
    } else if (mark == 0) { // 扩容
      return fillByExpansion();
    } else { // mark>0 && mark<=count 移动
      for (int indexOfBuffer = mark; indexOfBuffer < count; indexOfBuffer++) {
        buffer[indexOfBuffer - mark] = buffer[indexOfBuffer];
      }
      int oldCount = count - mark;
      int newCount = oldCount;
      for (int indexOfBuffer = oldCount; indexOfBuffer < buffer.length; indexOfBuffer++) {
        int read = doRead();
        if (eof == read) {
          break;
        }
        buffer[indexOfBuffer] = (byte) read;
        ++newCount;
      }
      this.count = newCount;
      this.nextPos -= mark;
      this.mark = 0;
      return newCount <= count ? false : true;
    }
  }

  private boolean fillByExpansion() {
    int nsz = count + standardBufferCapacity;
    byte[] nBuffer = new byte[nsz];
    System.arraycopy(buffer, 0, nBuffer, 0, count);
    int newCount = count;
    for (int indexOfNewBuffer = count; indexOfNewBuffer < nBuffer.length; indexOfNewBuffer++) {
      int read = doRead();
      if (eof == read) {
        break;
      }
      nBuffer[indexOfNewBuffer] = (byte) read;
      ++newCount;
    }
    this.buffer = nBuffer;
    this.count = newCount;
    return newCount <= count ? false : true;
  }

  private boolean fillByNoMark() {
    int read = doRead();
    if (eof == read) {
      return false;
    }
    mark = eof;
    nextPos = 0;
    count = 0;

    int indexOfFill = 0;
    buffer[indexOfFill++] = (byte) read;
    ++count;

    while (indexOfFill < buffer.length) {
      read = doRead();
      if (eof == read) {
        break;
      }
      buffer[indexOfFill++] = (byte) read;
      ++count;
    }
    return true;
  }

  /**
   * 跳过skipSteps.
   *
   * @param skipSteps 跳过的步数
   */
  public void skip(int skipSteps) {
    for (int skipTimes = 0; skipTimes < skipSteps; skipTimes++) {
      read();
    }
  }

  /** 重置到mark的位置. */
  public void reset() {
    if (mark == eof) {
      return;
    }
    nextReadIndex = nextReadIndex - (nextPos - mark);
    nextPos = mark;
    mark = eof;
  }

  public void mark() {
    this.mark = nextPos;
  }

  private int doRead() {
    int read = eof;
    try {
      read = byteInputStream.read();
    } catch (IOException e) {
      throw new CyanaAstRuntimeException(e);
    }
    return read;
  }

  /** 关闭输入流. */
  public void close() {
    try {
      byteInputStream.close();
    } catch (IOException e) {
      throw new CyanaAstRuntimeException(e);
    }
  }
}
