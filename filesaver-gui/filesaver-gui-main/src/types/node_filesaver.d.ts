declare module "node_filesaver" {
  export class FileSaver {
    start(): void;
    stop(): void;
    scan(target: string): void;
    getCurrentSizeAt(target: string): number;
    getTotalFiles(): number;
    getTotalKnownFiles(): number;
    getElapsed(): number;
    getFilesPerSecond(): number;
    getNumWorkers(): number;
    getStorageQueueSize(): number;
    getInMemoryEntryCount(): number;
  }
}
